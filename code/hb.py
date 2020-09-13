# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph
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
		return self.mat,self.size,self.to_edges

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
			if trace[i][1] == trace[i+1][1]:
				# self.sb_edges.append((trace[i][0],trace[i+1][0]))
				self.mat.addEdge(trace[i][0],trace[i+1][0])

	def sw(self,trace):

		write_models = [REL, ACQ_REL, SEQ_CST]
		read_models = [ACQ, ACQ_REL, SEQ_CST]

		for i in range(len(trace)):
			# create sw's between thread create statements
			if trace[i][2] == CREATE:
				v1 = trace[i][0]
				v2 = v1+1
				self.mat.addEdge(v1,v2)
				# self.sw_edges.append((v1,v2))
				self.to_edges.append((v1,v2))


			# create sw's between thread finish and join statements
			if trace[i][2] == JOIN:
				t = trace[i][5]			# value/number of the thread which is getting joined
				for j in range(i,len(trace)):
					if trace[j][2] == FINISH and trace[j][1] == t:
						v1 = trace[j][0]
						v2 = trace[i][0]
						# self.sw_edges.append((v1,v2))
						self.mat.addEdge(v1,v2)
						self.to_edges.append((v1,v2))

			# create sw's between read/rmw and write/rmw statements
			if trace[i][2] == READ or trace[i][2] == RMW:
				rf = trace[i][6]
				for j in trace:
					if j[0] == rf:
						if j[3] in write_models and trace[i][3] in read_models:
							# self.sw_edges.append((j[0],trace[i][0]))
							self.mat.addEdge(j[0],trace[i][0])
							if j[3] == SEQ_CST and trace[i][3] == SEQ_CST:
								self.to_edges.append((j[0],trace[i][0]))
