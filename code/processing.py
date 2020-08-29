# --------------------------------------------------------
# Processes the std:out data from the command line output
# and curates lists of:
#   1. Order of events including pseudo fences
#   2. Happens-before relationships (HB)
#   3. Modification Order relationships (MO)
#   4. Sequenced-before relationships (SB)
#   5. Total order relationships (TO)
#
# Then proceeds on to find out cycles from the TO graph
# and finally insert the required fences in the input
# program.
# --------------------------------------------------------

from hb import hb
from graph import Graph
from mo import mo
from to import to
from cycle import Cycles
from insert import insert
from z3translate import z3translate
from z3convert import z3convert

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
		self.cycles_total = 0

		trace_no = 0

		for trace in traces:											# run for each trace
			self.fence_thread = []										# list of fences separated by threads
			self.fence_sc_event_thread = []								# list of all fences and events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between fences as well as sc events
			self.cycles = []                                            # list of all cycles between the fences and events
			self.loc_info = {}                                          # information regarding the required fence locations

			trace_no += 1
			# print("---------Trace",trace_no,"---------")

			hb_time = time.time()
			hb_graph = hb(trace)
			mat,size = hb_graph.get()
			hb_time = time.time() - hb_time
			self.hb_total += hb_time

			mo_time = time.time()
			get_mo = mo(trace,mat,size)
			mo_edges = get_mo.get()
			# print("mo===",mo_edges)
			mo_time = time.time() - mo_time
			self.mo_total += mo_time
			
			fences_time = time.time()
			order=self.fence(trace)
			# print("order=",order)
			fences_time = time.time() - fences_time
			self.fences_total += fences_time

			to_time = time.time()
			to(order,mo_edges,self.sc_sb_edges)
			to_time = time.time() - to_time
			self.to_total += to_time

			cycles_time = time.time()
			cycles = Cycles()
			# print("no cycles=",len(cycles))
			cycles_time = time.time() - cycles_time
			self.cycles_total += cycles_time

			unique_fences = list(sorted(set(x for l in cycles for x in l)))
			unique_fences = [uf for uf in unique_fences if 'F' in uf]
			# print("unique_fences=",unique_fences)

			if len(unique_fences)>0:
				for fence in unique_fences:
					for i in range(len(order)):
						if fence == order[i]:
							o = order[i-1]
							fence_name = order[i]
							var_name = 'l'+str(o[8])
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


	def fence(self,trace):

		# find out the number of threads in the program
		threads = int(trace[-1][1])

		order = [] # IDEA: any var with _thread at the end means that it is separated by thread number

		for j in range(1,threads+1):
			fence_no = 0
			fences_events_in_thread = []
			fences_events_in_thread_min = []									# a minimal version of the above in order to find sb's

			fence_no+=1
			fence_name = 'F'+str(j)+'n'+str(fence_no)
			# IDEA: so many extra vars so that- use only where needed cuz loops increase complexity
			fences_events_in_thread.append(fence_name)
			order.append(fence_name)
			fences_events_in_thread_min.append(fence_name)

			for i in range(len(trace)):
				if int(trace[i][1])==j:

					if trace[i][2] == "fence":
						self.fences_in_trace.append(fence_name)
						continue
					else:
						if trace[i][3] == 'seq_cst':
							fences_events_in_thread_min.append(trace[i][0])			# event added to fences+sc events order in a thread

						order.append(trace[i])
						fences_events_in_thread.append(trace[i])

						fence_no+=1
						fence_name = 'F'+str(j)+'n'+str(fence_no)
						order.append(fence_name)
						fences_events_in_thread.append(fence_name)					# fence added to fences+all events order in a thread
						fences_events_in_thread_min.append(fence_name)				# fence added to fences+sc events order in a thread

			self.fence_sc_event_thread.append(fences_events_in_thread_min)

		# now find all sb relations between fences and events
		self.sb()

		return order

	def sb(self):

		for i in self.fence_sc_event_thread:
			for j in range(len(i)):
				for k in range(j+1,len(i)):
					sb_relation = (i[j],i[k])
					if not sb_relation in self.sc_sb_edges:
						self.sc_sb_edges.append(sb_relation)

		self.sc_sb_edges.sort(key = lambda x: x[0])

	def get(self):
		return self.loc_info, self.error_string, [self.hb_total, self.mo_total, self.fences_total, self.to_total, self.cycles_total]
