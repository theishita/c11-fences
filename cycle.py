import networkx as nx
import matplotlib.pyplot as plt
import itertools

def Cycles(sb_edges,to_edges):

	l=[]
	for a in sb_edges:
		l.append(a)
	for a in to_edges:
		l.append(a)
	# print(l)

	# Create Directed Graph with a lost of all edges
	G=nx.DiGraph(l)

	# Return a list of cycles described as a list o nodes
	cycles = list(nx.simple_cycles(G))

	# Sort the list items
	for i in range(len(cycles)):
		cycles[i] = sorted(cycles[i])
	cycles.sort()

	# Remove duplicate values
	cycles = list(cycles for cycles,_ in itertools.groupby(cycles))

	return cycles
