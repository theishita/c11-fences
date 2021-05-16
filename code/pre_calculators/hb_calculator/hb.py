# --------------------------------------------------------

# --------------------------------------------------------

from pre_calculators.graph import Graph
from constants import *

class hb:

	def __init__(self,trace):
		# print(trace)
		# self.sb_edges = []									# list of all sb edges between instructions
		self.sw_edges = []									# list of all sw edges between instructions
		self.to_edges = []									# list of to edges between instructions
		self.size = max(map(lambda x: x[0], trace))			# number of instructions in the execution trace
		self.size += 1
		self.mat = Graph(self.size)

		self.sb(trace)
		self.sw(trace)

		self.complete_matrix()

	def get(self):
		return self.mat, self.size, self.to_edges

	# adds all transitive relations
	def complete_matrix(self):

		temp = Graph(self.size)
		temp.adjMatrix = self.mat.adjMatrix
		flag = 0

		# print("t=",temp.adjMatrix)

		while flag!=2:
			for i in range(self.size):
				v1 = i
				for j in range(self.size):
					if(self.mat.containsEdge(v1,j)):
						for k in range(self.size):
							if(self.mat.containsEdge(j,k)):
								self.mat.addEdge(v1,k)

			if(temp.adjMatrix == self.mat.adjMatrix):
				flag += 1

	def sb(self,trace):

		# getting a list of sb as tuples of two
		for i in range(len(trace)-1):
			if trace[i][T_NO] == trace[i+1][T_NO]:
				# self.sb_edges.append((trace[i][S_NO],trace[i+1][S_NO]))
				self.mat.addEdge(trace[i][S_NO],trace[i+1][S_NO])

	def sw(self,trace):

		write_models = [REL, ACQ_REL, SEQ_CST]
		read_models = [ACQ, ACQ_REL, SEQ_CST]

		for i in range(len(trace)):
			# create sw's between thread create statements
			if trace[i][TYPE] == CREATE:
				v1 = trace[i][S_NO]
				v2 = v1+1
				self.mat.addEdge(v1,v2)
				# self.sw_edges.append((v1,v2))
				self.to_edges.append((v1,v2))


			# create sw's between thread finish and join statements
			if trace[i][TYPE] == JOIN:
				t = trace[i][VALUE]
				for j in range(i,len(trace)):
					if trace[j][TYPE] == FINISH and trace[j][T_NO] == t:
						v1 = trace[j][S_NO]
						v2 = trace[i][S_NO]
						# self.sw_edges.append((v1,v2))
						self.mat.addEdge(v1,v2)
						self.to_edges.append((v1,v2))

			# create sw's between read/rmw and write/rmw statements - based on rf
			if trace[i][TYPE] == READ or trace[i][TYPE] == RMW:
				rf = trace[i][RF]
				for j in trace:
					if j[S_NO] == rf:
						if j[MO] in write_models and trace[i][MO] in read_models:
							# self.sw_edges.append((j[S_NO],trace[i][S_NO]))
							self.mat.addEdge(j[S_NO],trace[i][S_NO])
							# if they are of memory order sc, then they need to also be counted as TO edges
							if j[MO] == SEQ_CST and trace[i][MO] == SEQ_CST:
								self.to_edges.append((j[S_NO],trace[i][S_NO]))
