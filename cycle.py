import networkx as nx
import matplotlib.pyplot as plt
import itertools

class Cycles:

	def __init__(self,nodes,sb_edges,to_edges):

		# Create Directed Graph
		G=nx.DiGraph()

		# Add a list of nodes:
		G.add_nodes_from(nodes)

		# Add a list of edges:
		G.add_edges_from(sb_edges)
		G.add_edges_from(to_edges)

		# Return a list of cycles described as a list o nodes
		self.cycles = list(nx.simple_cycles(G))

		# Remove duplicate list items
		for i in range(len(self.cycles)):
			self.cycles[i] = sorted(self.cycles[i])
		self.cycles.sort()
		self.cycles = list(self.cycles for self.cycles,_ in itertools.groupby(self.cycles))

		# print("cycles=",self.cycles)

	def get(self):
		return self.cycles
