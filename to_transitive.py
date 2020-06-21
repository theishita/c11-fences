# --------------------------------------------
# 
#---------------------------------------------

import ast
import networkx as nx

def to_transitive(cycles):

	# read all basic to_edges from the store
	to_edges_basic_store = open("store/to_edges_basic_store",'r')
	to_edges_basic = to_edges_basic_store.read()
	to_edges_basic_store.close()

	to_edges_basic = ast.literal_eval(to_edges_basic)
	to_edges_G = nx.DiGraph(to_edges_basic)

	to_sets = {}															# to store all the fences that caused each relations

	for cycle in cycles:
		n1 = cycle[0]
		n2 = cycle[1]

		all_paths_1 = list(nx.all_simple_paths(to_edges_G, source=n1, target=n2))
		all_paths_2 = list(nx.all_simple_paths(to_edges_G, source=n2, target=n1))

		fence_paths_1 = []
		fence_paths_2 = []

		for path in all_paths_1:
			new_path = sorted([p for p in path if 'F' in p])
			fence_paths_1.append(new_path)

		for path in all_paths_2:
			new_path = sorted([p for p in path if 'F' in p])
			fence_paths_2.append(new_path)
		
		to_sets[(n1,n2)] = fence_paths_1
		to_sets[(n2,n1)] = fence_paths_2

	# add all the sets to the store
	to_sets_store = open("store/to_sets_store", 'w')
	to_sets_store.write(str(to_sets))
	to_sets_store.close()
