import networkx as nx
import matplotlib.pyplot as plt
import itertools
import ast

def Cycles():

	to_edges_basic_store = open("store/to_edges_basic_store",'r')
	to_edges = to_edges_basic_store.read()
	to_edges_basic_store.close()

	to_edges = ast.literal_eval(to_edges)

	# # Create Directed Graph with a lost of all edges
	G=nx.DiGraph(to_edges)

	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	for i in range(len(cycles)):
		cycles[i] = [c for c in cycles[i] if 'F' in c]

	# cycles = [c for c in cycles if len(c)>1]

	# Sort the list items
	# for i in range(len(cycles)):
		# cycles[i] = sorted(cycles[i])
	# cycles.sort()

	# Remove duplicate values and cycles of length 1
	# cycles = list(cycles for cycles,_ in itertools.groupby(cycles))
	# cycles = [c for c in cycles if len(c)>1]

	return cycles
