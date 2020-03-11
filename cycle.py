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

		# print("cycles=",self.cycles)

	def get(self):
		return self.cycles
