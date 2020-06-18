import networkx as nx
import matplotlib.pyplot as plt
import itertools
import ast

def Cycles():

	to_edges_transitive_store = open("store/to_edges_transitive_store",'r')
	to_edges = to_edges_transitive_store.read()
	to_edges_transitive_store.close()

	to_edges = ast.literal_eval(to_edges)

	# # Create Directed Graph with a lost of all edges
	# G=nx.DiGraph(to_edges)

	# Return a list of cycles described as a list o nodes
	cycles = []
	for edge0 in to_edges:
		first_edge0 = edge0[0]
		first_edge1 = edge0[1]

		if first_edge0 != first_edge1:
			for edge1 in to_edges:
				last_edge0 = edge1[0]
				last_edge1 = edge1[1]

				if first_edge1 == last_edge0 and first_edge0 == last_edge1:
					cycles.append(edge0)

	# cycles = [c for c in cycles if len(c)>1]

	# Sort the list items
	for i in range(len(cycles)):
		cycles[i] = sorted(cycles[i])
	cycles.sort()

	# Remove duplicate values and cycles of length 1
	cycles = list(cycles for cycles,_ in itertools.groupby(cycles))
	cycles = [c for c in cycles if len(c)>1]

	return cycles
