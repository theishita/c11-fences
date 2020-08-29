import networkx as nx
import ast

def Cycles(to_edges):

	# Create Directed Graph with a lost of all edges
	G=nx.DiGraph(to_edges)

	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	for i in range(len(cycles)):
		cycles[i] = [c for c in cycles[i] if 'F' in c]

	return cycles
