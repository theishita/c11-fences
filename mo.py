# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class mo:

	def __init__(self,mat,vertex_map,instr,size):
		self.mat = mat
		self.vertex_map = vertex_map
		self.vertex_map_swap = dict([(value, key) for key, value in self.vertex_map.items()])			# swap keys with values for easier usage while finding MO's
		self.instr = instr
		self.size = size

		self.mo_edges = []                      														# list of mo edges

		self.rule1()
		self.rule2()
		self.rule3()
		self.rule4()
		self.mo_edges = list(dict.fromkeys(self.mo_edges))
		# print("mo edges=",self.mo_edges)

	def get(self):
		return self.mo_edges

	def rule1(self):
		for i in range(self.size):
			for j in range(self.size):
				if self.mat.containsEdge(i,j):
					# print(vertex_map[i],vertex_map[j],1)
					a = self.vertex_map_swap[i]
					b = self.vertex_map_swap[j]
					flag = 0																			# set flag for checking if both are write commands

					for k in self.instr:
						if k['no'] == a and k['type'] == 'write':
							a_var = k['loc']															# check if the variable is the same
							flag += 1
						if k['no'] == b and k['type'] == 'write':
							b_var = k['loc']
							flag += 1

					if flag == 2 and a_var == b_var:
						self.mo_edges.append((a,b))

	def rule2(self):
		for i in self.instr:
			if i['type'] == 'read':
				a_no = i['no']

				for j in self.instr:
					if j['type'] == 'read':
						b_no = j['no']
						a_var = i['loc']
						b_var = j['loc']

						if a_var == b_var:
							a = self.vertex_map[a_no]
							b = self.vertex_map[b_no]

							if self.mat.containsEdge(a,b):
								x = i['rf']
								y = j['rf']

								if not x == y:
									self.mo_edges.append((x,y))

	def rule3(self):
		# print(self.vertex_map)
		for i in self.instr:
			if i['type'] == 'read':
				a_no = i['no']

				for j in self.instr:
					if j['type'] == 'write':
						b_no = j['no']

						a_var = i['loc']
						b_var = j['loc']

						if a_var == b_var:
							a = self.vertex_map[a_no]
							b = self.vertex_map[b_no]

							if self.mat.containsEdge(a,b):
								x = i['rf']
								self.mo_edges.append((x,b_no))

	def rule4(self):
		for i in self.instr:
			if i['type'] == 'write':
				x_no = i['no']

				for j in self.instr:
					if j['type'] == 'read':
						b_no = j['no']

						x_var = i['loc']
						b_var = j['loc']

						if x_var == b_var:
							x = self.vertex_map[x_no]
							b = self.vertex_map[b_no]

							if self.mat.containsEdge(x,b):
								b_rf = j['rf']

								if not b_rf == x_no:
									self.mo_edges.append((x_no,b_rf))
