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

class Processing:
	def __init__(self,traces,no_traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []											# list of disjunctions for the z3 function
		self.fences_present = []										# list of fences converted to their respective variable names
		self.error_string = ''

		trace_no = 0

		for t in range(no_traces):										# run for each trace
			trace = traces[t]
			self.fence_sc_event_thread = []								# list of all fences and events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between fences as well as sc events
			self.fences_in_trace = []									# list of fences already present in the program
			self.cycles = []                                            # list of all cycles between the fences and events
			self.loc_info = {}                                          # information regarding the required fence locations

			# trace_no += 1
			# print("trace=",trace_no)

			hb_graph = hb(trace)
			mat,size = hb_graph.get()

			get_mo = mo(trace,mat,size)
			mo_edges = get_mo.get()
			# print("mo===",mo_edges)

			order=self.fence(trace)
			# print("order==",order)

			to(order,mo_edges,self.sc_sb_edges)

			cycles = Cycles()
			# print("no cycles=",len(cycles))

			# to_transitive(cycles)

			unique_fences = list(sorted(set(x for l in cycles for x in l)))
			unique_fences = [uf for uf in unique_fences if 'F' in uf]
			# print("unique_fences=",unique_fences)

			if len(unique_fences)>0:
				for fence in unique_fences:
					# create unique variable names for z3 based on line numbers
					for i in range(len(order)):
						if fence == order[i]:
							o = order[i-1]
							var_name = 'l'+str(o['line'])
							self.loc_info[fence] = var_name
					
					# check for the fences already present in input prgm and replace them with these variables
					if (fence in self.fences_in_trace) and (var_name not in self.fences_present):
						self.fences_present.append(var_name)

				get_translation = z3translate(cycles,self.loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no+1)+"\nHence this behaviour cannot be stopped using SC fences"
				return

		z3convert(self.z3vars,self.disjunctions,self.fences_present)


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
						event = {'no': trace[i][0],									# trace[i][0] is the event number
								'thread': j,
								'mo': trace[i][3],
								'type': trace[i][2]
						}
						if trace[i][2]=='read' or trace[i][2]=="rmw":
							event['rf'] = trace[i][6]								# trace[i][7] gives Read-from (Rf)
							event['loc'] = trace[i][4]
							event['line'] = trace[i][8]								# line number in the original source code
						elif trace[i][2]=='write':
							event['loc'] = trace[i][4]
							event['line'] = trace[i][8]

						if event['mo'] == 'seq_cst':
							fences_events_in_thread_min.append(trace[i][0])			# event added to fences+sc events order in a thread

						order.append(event)
						fences_events_in_thread.append(event)

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
		return self.fences_present, self.error_string