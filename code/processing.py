# --------------------------------------------------------
# Processes the std:out data from the command line output
# and curates lists of:
#   1. Order of events including pseudo fences
#   2. Happens-before relationships (HB)
#   3. Modification Order relationships (MO)
#   4. Sequenced-before relationships (SB)
#   5. Total order relationships (TO)
#
# Then proceeds on to find out cycles from the TO graph.
# --------------------------------------------------------

from pre_calculators.hb_calculator.hb import hb
from pre_calculators.mo_calculator.mo import mo
from preprocessing import preprocessing
from edges_computation import edges_computation
from to import to
from cycle import Cycles
from z3translate import z3translate
from constants import *

import sys
import time

# IDEA: any var with _thread at the end means that it is an array of arrays separated by thread number
class Processing:
	def __init__(self, traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []											# list of disjunctions for the z3 function
		self.fences_present = []										# list of fences converted to their respective variable names
		self.fences_present_locs = []
		self.error_string = ''
		self.pre_calc_total = 0									# time taken for calculation of initial values - HB, MO, SB
		self.hb_cycles = []
		self.so_cycles = []

		trace_no = 0
		# print("traces=",traces)

		for trace in traces:										# run for each trace
			self.all_sc_events_thread = []								# list of all sc events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between all sc events
			self.fences_thread = []										# list of fences in each thread
			self.fences_in_trace = []									# list of fences already present in the program
			self.so_edges = []											# list of all SO edge tuples
			self.hb_edges = []											# list of all SW+DOB+WS edge tuples
			self.cycles = []                        # list of all cycles between the fences and events
			loc_info = {}                                          # information regarding the required fence locations

			trace_no += 1
			# print("---------Trace",trace_no,"---------")

			pre_calc_start = time.time()
			# HB
			hb_graph = hb(trace)
			hb_matrix, size, self.so_edges = hb_graph.get()

			# MO
			get_mo = mo(trace, hb_matrix, size, self.so_edges)
			mo_edges, self.so_edges = get_mo.get()
			# print("mo =",mo_edges)
			pre_calc_end = time.time()
			self.pre_calc_total += (pre_calc_end-pre_calc_start)

			# ADD FENCES
			order=self.fence(trace)
			# print("order =",order)
			# print("fences_present =", self.fences_in_trace)
			# print("fences_thread =", self.fences_thread)

			# transitive SB
			self.sb()
			self.so_edges += self.sc_sb_edges

			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)

			# TO
			# calc_to = to(order, reads, writes, self.fences_thread, mo_edges, self.so_edges)
			# self.so_edges = calc_to.get()
			# print("to =",self.so_edges)

			# CALC EDGES
			calc_edges = edges_computation(order, reads, writes, self.fences_thread, self.so_edges, mo_edges)
			calc_edges.compute_all_edges()
			self.so_edges, self.hb_edges = calc_edges.get()
			# print("so = ", self.so_edges)
			# print("hb = ", self.hb_edges)
			
			# CYCLES
			self.so_cycles = Cycles(self.so_edges)
			self.hb_cycles = Cycles(self.hb_edges)
			# hb_cycles = []
			cycles = self.so_cycles + self.hb_cycles
			cycles = [list(item) for item in set(tuple(row) for row in cycles)] # removing duplicate values
			# print("no cycles=",len(cycles))
			# print("cycles =",cycles)

			cycles_with_only_fences = []
			for i in range(len(cycles)):
				cycles_with_only_fences.append([c for c in cycles[i] if type(c) is str])
			cycles_with_only_fences = [list(item) for item in set(tuple(sorted(row)) for row in cycles_with_only_fences)] # removing duplicate values
			unique_fences = list(sorted(set(x for l in cycles_with_only_fences for x in l)))
			# print("unique_fences=",unique_fences)
			# print("cycles_with_only_fences =",cycles_with_only_fences)

			if len(unique_fences)>0:
				for fence in unique_fences:
					i = order.index(fence)
					fence_name = order[i]
					var_name = 'l'+str(order[i-1][LINE_NO])
					loc_info[fence_name] = var_name
					
					# check for the fences already present in input prgm and replace them with these variables
					if (fence in self.fences_in_trace) and (var_name not in self.fences_present):
						self.fences_present.append(var_name)
						self.fences_present_locs.append(order[i-1][LINE_NO])
			
				# print("fences loc_info =",loc_info)

				get_translation = z3translate(cycles_with_only_fences, loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no)+"\nHence this behaviour cannot be stopped using SC fences\n"
				return

	def fence(self, trace):
		order = ['F1n1']								
		sc_events = ['F1n1']
		fences_in_thread = ['F1n1']

		current_thread = 1				# for fence naming
		fence_no = 2

		for i in range(len(trace)):
			if trace[i][T_NO] != current_thread:
				self.all_sc_events_thread.append(sc_events)
				self.fences_thread.append(fences_in_thread)

				sc_events = []
				fences_in_thread = []
				current_thread += 1
				fence_no = 1
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
				fence_no += 1
				order.append(fence_name)
				sc_events.append(fence_name)
				fences_in_thread.append(fence_name)

			if not trace[i][TYPE] == FENCE:
				order.append(trace[i])
				if trace[i][MO] == SEQ_CST:
					sc_events.append(trace[i][S_NO])
			else:
				self.fences_in_trace.append(fence_name)
				if i == (len(trace)-1):
					self.all_sc_events_thread.append(sc_events)
					self.fences_thread.append(fences_in_thread)
				continue

			if trace[i][LINE_NO] != 'NA':
				fence_name = 'l'+str(trace[i][LINE_NO])
			else:
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
			fence_no += 1

			order.append(fence_name)
			sc_events.append(fence_name)
			fences_in_thread.append(fence_name)
			
			if i == (len(trace)-1):
				self.all_sc_events_thread.append(sc_events)
				self.fences_thread.append(fences_in_thread)

		return order

	def sb(self):
		for i in self.all_sc_events_thread:
			for j in range(len(i)):
				for k in range(j+1, len(i)):
					self.sc_sb_edges.append((i[j],i[k]))

	def get(self):
		return self.fences_present, self.fences_present_locs, self.z3vars, self.disjunctions, self.error_string, self.pre_calc_total, self.hb_cycles, self.so_cycles, self.hb_edges
