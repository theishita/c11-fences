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

from hb import hb
from graph import Graph
from mo import mo
from to import to
from cycle import Cycles
from z3translate import z3translate
from z3convert import z3convert
from constants import *

import sys
import time

class Processing:
	def __init__(self,traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []											# list of disjunctions for the z3 function
		self.error_string = ''
		self.hb_total = 0
		self.mo_total = 0
		self.fences_total = 0
		self.to_total = 0
		self.sb_total = 0
		self.cycles_total = 0
		self.trans_time = 0

		trace_no = 0

		for trace in traces:											# run for each trace
			self.all_sc_events_thread = []								# list of all sc events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between all sc events
			self.to_edges = []											# list of all TO edge tuples
			self.cycles = []                                            # list of all cycles between the fences and events
			self.loc_info = {}                                          # information regarding the required fence locations

			trace_no += 1
			# print("---------Trace",trace_no,"---------")

			# HB
			hb_time = time.time()
			hb_graph = hb(trace,self.trans_time)
			mat,size,self.to_edges,self.trans_time = hb_graph.get()
			hb_time = time.time() - hb_time
			self.hb_total += hb_time
			
			# MO
			mo_time = time.time()
			get_mo = mo(trace,mat,size)
			mo_edges = get_mo.get()
			mo_time = time.time() - mo_time
			# print("mo =",mo_edges)
			self.mo_total += mo_time
			
			# ADD FENCES
			fences_time = time.time()
			order=self.fence(trace)
			fences_time = time.time() - fences_time
			# print("order =",order)
			self.fences_total += fences_time

			# transitive SB
			sb_time = time.time()
			self.sb()
			sb_time = time.time() - sb_time
			self.sb_total += sb_time

			# TO
			to_time = time.time()
			calc_to = to(order,mo_edges,self.sc_sb_edges,self.to_edges)
			self.to_edges = calc_to.get()
			to_time = time.time() - to_time
			# print("TO=",self.to_edges)
			self.to_total += to_time
			
			# CYCLES
			cycles_time = time.time()
			cycles = Cycles(self.to_edges)
			# print("no cycles=",len(cycles))
			cycles_time = time.time() - cycles_time
			self.cycles_total += cycles_time

			unique_fences = list(sorted(set(x for l in cycles for x in l)))
			unique_fences = [uf for uf in unique_fences if 'F' in uf]
			# print("unique_fences=",unique_fences)
			# print("order=",order)

			if len(unique_fences)>0:
				for fence in unique_fences:
					i = order.index(fence)
					fence_name = order[i]
					var_name = 'l'+str(order[i-1][8])
					self.loc_info[fence_name] = var_name
				
				get_translation = z3translate(cycles,self.loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no)+"\nHence this behaviour cannot be stopped using SC fences\n"
				return

		z3convert(self.z3vars,self.disjunctions)
		print("hb trans time=",self.trans_time)

	def fence(self, trace):
		order = []								
		sc_events = []					# IDEA: any var with _thread at the end means that it is separated by thread number

		current_thread = 1				# for fence naming
		fence_no = 1

		for i in range(len(trace)):
			fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
			fence_no += 1

			order.append(fence_name)
			sc_events.append(fence_name)

			if trace[i][1] != current_thread:
				self.all_sc_events_thread.append(sc_events)
				sc_events = []
				current_thread += 1
				fence_no = 1
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
				fence_no += 1
				order.append(fence_name)
				sc_events.append(fence_name)

			order.append(trace[i])
			if trace[i][3] == SEQ_CST:
				sc_events.append(trace[i][0])
			
			if i == (len(trace)-1):
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
				fence_no += 1
				order.append(fence_name)
				sc_events.append(fence_name)
				self.all_sc_events_thread.append(sc_events)

		return order

	def sb(self):
		for i in self.all_sc_events_thread:
			for j in range(len(i)):
				for k in range(j+1, len(i)):
					self.sc_sb_edges.append((i[j],i[k]))

	def get(self):
		return self.loc_info, self.error_string, [self.hb_total, self.mo_total, self.fences_total, self.sb_total, self.to_total, self.cycles_total]
