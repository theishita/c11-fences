import networkx as nx
import ast

def Cycles(to_edges):

	# Create Directed Graph with a list of all edges
	G=nx.DiGraph(to_edges)

	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	return cycles
