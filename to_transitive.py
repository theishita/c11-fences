# --------------------------------------------
# 
#---------------------------------------------

import ast
import networkx as nx

def to_transitive():

	# read all basic to_edges from the store
	to_edges_basic_store = open("store/to_edges_basic_store",'r')
	to_edges_basic = to_edges_basic_store.read()
	to_edges_basic_store.close()

	to_edges_basic = ast.literal_eval(to_edges_basic)
	to_edges_G = nx.DiGraph(to_edges_basic)

	# read all basic to_edges from the store
	to_edges_transitive_store = open("store/to_edges_transitive_store",'r')
	to_edges_transitive = to_edges_transitive_store.read()
	to_edges_transitive_store.close()

	to_edges_transitive = ast.literal_eval(to_edges_transitive)

	to_sets = {}															# to store all the fences that caused each relations

	for edge in to_edges_transitive:
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
