import networkx as nx
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

	return cycles
