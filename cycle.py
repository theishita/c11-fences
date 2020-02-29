import networkx as nx
import matplotlib.pyplot as plt

class Cycles:

	def __init__(self,nodes,sb_edges,to_edges):
		
		# Create Directed Graph
		G=nx.DiGraph()

		# Add a list of nodes:
		G.add_nodes_from(nodes)

		# Add a list of edges:
		G.add_edges_from(sb_edges)
		G.add_edges_from(to_edges)

		#Return a list of cycles described as a list o nodes
		self.cycles = list(nx.simple_cycles(G))
		self.smallest = []

		print("cycles=",self.cycles)

		if len(self.cycles) > 0:
			self.smallest = self.detect_smallest()
	
	def get(self):
		return self.smallest

	def detect_smallest(self):
		smallest = self.cycles[0]
		
		for i in self.cycles:
			if len(i)<len(smallest):
				smallest = i
		
		return smallest