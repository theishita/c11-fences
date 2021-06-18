def get_fence_types(req_fences, hb_cycles, so_cycles, hb_edges):
	# print("req_fences=", req_fences)
	print("hb_cycles=", hb_cycles)
	# print("so_cycles=", so_cycles)
	# print("hb_edges=", hb_edges)

	approved_hb_cycles = []
	approved_so_cycles = []

	for hb_cycle in hb_cycles:
		only_fences_cycle = [c for c in hb_cycle if type(c) is str]
		flag = 0
		for el in only_fences_cycle:
			if el not in req_fences:
				flag = 1
		
		if flag == 0:
			approved_hb_cycles.append(hb_cycle)

	for so_cycle in so_cycles:
		only_fences_cycle = [c for c in so_cycle if type(c) is str]
		flag = 0
		for el in only_fences_cycle:
			if el not in req_fences:
				flag = 1
		
		if flag == 0:
			approved_so_cycles.append(so_cycle)
	
	print(approved_hb_cycles)