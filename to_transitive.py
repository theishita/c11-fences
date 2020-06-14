# --------------------------------------------
# 
#---------------------------------------------

import ast
import networkx as nx

def to_transitive():

	# read all basic to_edges from the store
	to_edges_store = open("store/to_edges_store",'r')
	to_edges = to_edges_store.read()
	to_edges_store.close()

	to_edges = ast.literal_eval(to_edges)

	to_edges_G = nx.DiGraph(to_edges)										# create a graph from the basic edges
	to_edges_Gt = nx.transitive_closure(to_edges_G, reflexive=False)		# obtain all the transitive relations between the edges as well

	to_sets = {}															# to store all the fences that caused each relations

	for edge in to_edges_Gt.edges:
		n1 = edge[0]
		n2 = edge[1]
		all_paths = list(nx.all_simple_paths(to_edges_G, source=n1, target=n2))
		fence_paths = []

		for path in all_paths:
			new_path = sorted([p for p in path if 'F' in p])
			if new_path not in fence_paths:
				fence_paths.append(new_path)
		
		to_sets[(edge)] = fence_paths

	# add all the sets to the store
	to_sets_store = open("store/to_sets_store", 'w')
	to_sets_store.write(str(to_sets))
	to_sets_store.close()

	# update the to_edges store by including all transitive edges
	to_edges_all = list(to_edges_Gt.edges)
	to_edges_store = open("store/to_edges_store",'w')
	to_edges_store.write(str(to_edges_all))
	to_edges_store.close()