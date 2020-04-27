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
from instructions_z3 import translate_z3
from stitch_z3 import convert_z3

import sys

class Processing:
	def __init__(self,traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []											# list of disjunctions for the z3 function

		trace_no = 0

		for trace in traces:											# run for each trace
			self.fence_thread = []										# list of fences separated by threads
			self.fence_sb_edges = []									# list of all sb edge pairs between fences
			self.cycles = []                                            # list of all cycles between the fences
			self.loc_info = []                                          # information regarding the required fence locations

			trace_no += 1

			hb_graph = hb(trace)
			mat,size = hb_graph.get()

			get_mo = mo(trace,mat,size)
			mo_edges = get_mo.get()

			order=self.fence(trace)

			get_to = to(order,mo_edges,self.fence_sb_edges)
			to_edges = get_to.get()
			# print("to_edges=",to_edges)

			# print("sb fence=",self.fence_sb_edges)
			# print("fences in sb=",self.fence_thread)

			cycles = Cycles(self.fence_sb_edges,to_edges)

			unique_fences = list(sorted(set(x for l in cycles for x in l)))

			if len(unique_fences)>0:
				for fence in unique_fences:
					for i in range(len(order)):
						if fence == order[i]:
							o = order[i-1]
							var_name = 'l'+str(o['line'])
							temp = {'line': o['line'],
									'var_name': var_name,
									'fence': order[i]}
							self.loc_info.append(temp)

				get_translation = translate_z3(cycles,self.loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				print("No TO cycles can be formed for trace",trace_no,"\nHence this behaviour cannot be stopped using SC fences")
				sys.exit()

		convert_z3(self.z3vars,self.disjunctions)


	def fence(self,trace):

		# find out the number of threads in the program
		threads = int(trace[-1][1])

		exec = []

		for j in range(1,threads+1):
			fence_no = 0
			fences_in_thread = []
			for i in range(len(trace)):
				if int(trace[i][1])==j:
					fence_no+=1
					exec.append('F'+str(j)+'n'+str(fence_no))
					fences_in_thread.append('F'+str(j)+'n'+str(fence_no))		# fence order in a thread
					event = {'no': trace[i][0],									# trace[i][0] is the event number
							'thread': j,
					}
					if trace[i][2]=='read':
						event["type"] = "read"
						event['rf'] = trace[i][6]						# trace[i][7] gives Read-from (Rf)
						event['mo'] = trace[i][3]
						event['loc'] = trace[i][4]
						event['line'] = trace[i][9]						# line number in the original source code
					elif trace[i][2]=='write':
						event["type"] = "write"
						event['mo'] = trace[i][3]
						event['loc'] = trace[i][4]
						event['line'] = trace[i][9]
					exec.append(event)
			fence_no+=1
			exec.append('F'+str(j)+'n'+str(fence_no))
			fences_in_thread.append('F'+str(j)+'n'+str(fence_no))
			self.fence_thread.append(fences_in_thread)

		self.sb(exec,threads)

		return exec

	def sb(self, trace, threads):

		for i in self.fence_thread:
			for j in range(len(i)):
				for k in range(j+1,len(i)):
					if not (i[j],i[k]) in self.fence_sb_edges:
						self.fence_sb_edges.append((i[j],i[k]))

		self.fence_sb_edges.sort(key = lambda x: x[0])

	def get(self):
		return self.loc_info
