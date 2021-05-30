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

		trace_no = 0
		# print("traces=",traces)

		for trace in traces:										# run for each trace
			self.all_sc_events_thread = []								# list of all sc events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between all sc events
			self.fences_thread = []										# list of fences in each thread
			self.fences_in_trace = []									# list of fences already present in the program
			self.to_edges = []											# list of all TO edge tuples
			self.cycles = []                                            # list of all cycles between the fences and events
			self.loc_info = {}                                          # information regarding the required fence locations

			trace_no += 1
			# print("---------Trace",trace_no,"---------")

			pre_calc_start = time.time()
			# HB
			hb_graph = hb(trace)
			hb_matrix, size, self.to_edges = hb_graph.get()

			# MO
			get_mo = mo(trace, hb_matrix, size, self.to_edges)
			mo_edges, self.to_edges = get_mo.get()
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
			self.to_edges += self.sc_sb_edges

			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)

			# TO
			calc_to = to(order, reads, writes, self.fences_thread, mo_edges, self.to_edges)
			self.to_edges = calc_to.get()
			# print("to =",self.to_edges)
			
			# CYCLES
			cycles = Cycles(self.to_edges)
			# print("no cycles=",len(cycles))
			# print("cycles =",cycles)

			unique_fences = list(sorted(set(x for l in cycles for x in l)))
			unique_fences = [uf for uf in unique_fences if 'F' in uf]
			# print("unique_fences=",unique_fences)
			# converted_cycs = []
			if len(unique_fences)>0:
				for fence in unique_fences:
					i = order.index(fence)
					fence_name = order[i]
					var_name = 'l'+str(order[i-1][LINE_NO])
					self.loc_info[fence_name] = var_name
					
					# check for the fences already present in input prgm and replace them with these variables
					if (fence in self.fences_in_trace) and (var_name not in self.fences_present):
						self.fences_present.append(var_name)
						self.fences_present_locs.append(order[i-1][LINE_NO])
			
				# for cyc in cycles:
				# 	x = []
				# 	for c in cyc:
				# 		x.append(self.loc_info[c])
				# 	converted_cycs.append(x)
				# print("cycles =",converted_cycs)

				get_translation = z3translate(cycles, self.loc_info)
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
		return self.fences_present, self.fences_present_locs, self.z3vars, self.disjunctions, self.error_string, self.pre_calc_total
