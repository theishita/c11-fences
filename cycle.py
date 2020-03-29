import networkx as nx
import matplotlib.pyplot as plt
import itertools

class Cycles:

	def __init__(self,nodes,sb_edges,to_edges):

		l=[]
		for a in sb_edges:
			l.append(a)
		for a in to_edges:
			l.append(a)
		# print(l)

		# Create Directed Graph with a lost of all edges
		G=nx.DiGraph(l)

		# Return a list of cycles described as a list o nodes
		self.cycles = list(nx.simple_cycles(G))

		# Remove duplicate list items
		for i in range(len(self.cycles)):
			self.cycles[i] = sorted(self.cycles[i])
		self.cycles.sort()
		self.cycles = list(self.cycles for self.cycles,_ in itertools.groupby(self.cycles))

	def get(self):
		return self.cycles
