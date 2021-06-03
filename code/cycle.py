import networkx as nx
import ast

def Cycles(to_edges):

	# Create Directed Graph with a lost of all edges
	G=nx.DiGraph(to_edges)

	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	# print("cycles before removing instr = ", cycles);

	for i in range(len(cycles)):
		cycles[i] = [c for c in cycles[i] if type(c) is str]

	return cycles
