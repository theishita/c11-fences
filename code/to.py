# --------------------------------------------------------

# --------------------------------------------------------

import ast
from graph import Graph
import networkx as nx

class to:

	def __init__(self,order,mo_edges,sb_edges):
		self.order = order
		self.mo_edges = mo_edges
		self.sb_edges = sb_edges

		to_edges_basic_store = open("store/to_edges_basic_store",'r')
		to_edges_basic = to_edges_basic_store.read()
		to_edges_basic_store.close()
		to_edges_basic = ast.literal_eval(to_edges_basic)
		self.to_edges = list(set(to_edges_basic) | set(sb_edges))

		print("rule 0")
		self.rule0()
		print("rule 1a")
		self.rule1a()
		print("rule 2")
		self.rule2()
		print("rule 3, 1b")
		self.rule3_1b()
		print("rule 4")
		self.rule4()

		# write the basic to edges
		to_edges_basic_store = open("store/to_edges_basic_store",'w')
		to_edges_basic_store.write(str(self.to_edges))
		to_edges_basic_store.close()

		self.sort_to_edges()
		self.create_transitive_edges()

	def rule0(self):
		for i in range(len(self.order)):
			b = self.order[i]
			if "type" in b and (b['type'] == 'read' or b['type'] == 'rmw'):
				a_no = b['rf']

				y = self.order[i+1]

				for j in range(len(self.order)):
					a = self.order[j]
					if 'no' in a and a['no'] == a_no:
						x = self.order[j-1]
						
						if not (x,y) in self.to_edges:
							self.to_edges.append((x,y))
							print(self.to_edges[-1])


	def rule1a(self):
		for b in self.order:
			if 'type' in b and (b['type'] == 'read' or b['type'] == 'rmw') and b['mo'] == 'seq_cst':
				b_no = b['no']

				a_no = b['rf']

				for a in self.order:
					if 'no' in a and a['no'] == a_no and a['mo'] == 'seq_cst':

						if not (a_no,b_no) in self.to_edges:
							self.to_edges.append((a_no,b_no))
							print(self.to_edges[-1])

	def rule2(self):
		for i in self.mo_edges:
			m1_no = str(i[0])
			m2_no = str(i[1])

			for m2 in self.order:
				if 'type' in m2 and m2['no'] == m2_no:

					if m2['mo'] == 'seq_cst':
						for j in range(len(self.order)):
							b = self.order[j]
							if 'rf' in b and b['rf'] == m1_no:
								x = self.order[j-1]

								if not (x,m2_no) in self.to_edges:
									self.to_edges.append((x,m2_no))
									print(self.to_edges[-1])

	def rule3_1b(self):
		for i in self.mo_edges:
			m_no = str(i[0])
			a_no = str(i[1])
			x = 0
			y = []														# since there might be multiple read instructions with rf m_no
			b = []
			seq = 0

			for j in range(len(self.order)):
				if "rf" in self.order[j] and self.order[j]['rf'] == m_no:
					y.append(self.order[j-1])

					# checking also for rule 1b, if B is sequential
					if self.order[j]['mo'] == 'seq_cst':
						b.append(self.order[j]['no'])

				if 'type' in self.order[j] and self.order[j]['no'] == a_no:
					x = self.order[j+1]

					# checking also for rule 1b, if A is seq_cst
					if self.order[j]['mo'] == 'seq_cst':
						seq += 1

			# rule 1b
			if seq == 1 and b:
				for b_no in b:
					if not (b_no,a_no) in self.to_edges:
						self.to_edges.append((b_no,a_no))
						print(self.to_edges[-1])

			# rule 3
			if x and y:
				for fy in y:
					if not (fy,x) in self.to_edges:
						self.to_edges.append((fy,x))
						print(self.to_edges[-1])

	def rule4(self):
		for i in self.mo_edges:
			b_no = i[0]
			a_no = i[1]

			for j in range(len(self.order)):
				if 'type' in self.order[j] and self.order[j]['no'] == b_no:
					b = self.order[j]
					y = self.order[j-1]
				if 'type' in self.order[j] and self.order[j]['no'] == a_no:
					a = self.order[j]
					x = self.order[j+1]

			# rule 4a
			if b['mo'] == 'seq_cst':
				if not (b_no,x) in self.to_edges:
					self.to_edges.append((b_no,x))
					print(self.to_edges[-1])

			# rule 4b
			if a['mo'] == 'seq_cst':
				if not (y,a_no) in self.to_edges:
					self.to_edges.append((y,a_no))
					print(self.to_edges[-1])

			# rule 4c
			if not (y,x) in self.to_edges:
				self.to_edges.append((y,x))
				print(self.to_edges[-1])

	def sort_to_edges(self):
		# read from the store
		to_edges_basic_store = open("store/to_edges_basic_store",'r')
		to_edges = to_edges_basic_store.read()
		to_edges_basic_store.close()

		# convert contents of the file into readable list format
		to_edges = ast.literal_eval(to_edges)

		# sort and remove duplicates
		to_edges = list(set(to_edges))
		to_edges.sort(key = lambda x: x[0])

		# write back into the store
		to_edges_basic_store = open("store/to_edges_basic_store",'w')
		to_edges_basic_store.write(str(to_edges))
		to_edges_basic_store.close()
	
	def create_transitive_edges(self):
		to_edges_basic_store = open("store/to_edges_basic_store",'r')
		to_edges = to_edges_basic_store.read()
		to_edges_basic_store.close()

		to_edges = ast.literal_eval(to_edges)
