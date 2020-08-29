# --------------------------------------------------------

# --------------------------------------------------------

import ast
from graph import Graph
import networkx as nx

class to:

	def __init__(self,order,mo_edges,sb_edges,to_edges):
		self.order = order
		self.mo_edges = mo_edges
		self.sb_edges = sb_edges

		self.to_edges = to_edges + sb_edges

		self.rule0()
		self.rule1a()
		self.rule2()
		self.rule3_1b()
		self.rule4()

		self.to_edges = list(set(self.to_edges))		# remove repeats

		# write the basic to edges
		# to_edges_basic_store = open("store/to_edges_basic_store",'w')
		# to_edges_basic_store.write(str(self.to_edges))
		# to_edges_basic_store.close()

		# self.sort_to_edges()
		# self.create_transitive_edges()
	
	def get(self):
		return self.to_edges

	def rule0(self):
		for i in range(len(self.order)):
			b = self.order[i]
			if type(b) is list and (b[2] == 'read' or b[2] == 'rmw'):
				a_no = b[6]

				y = self.order[i+1]

				for j in range(len(self.order)):
					a = self.order[j]
					if type(a) is list and a[0] == a_no:
						x = self.order[j-1]
						
						if not (x,y) in self.to_edges:
							self.to_edges.append((x,y))


	def rule1a(self):
		for b in self.order:
			if (b[2] == 'read' or b[2] == 'rmw') and b[3] == 'seq_cst':
				b_no = b[0]
				a_no = b[6]

				for a in self.order:
					if a[0] == a_no and a[3] == 'seq_cst':

						if not (a_no,b_no) in self.to_edges:
							self.to_edges.append((a_no,b_no))

	def rule2(self):
		for i in self.mo_edges:
			m1_no = str(i[0])
			m2_no = str(i[1])

			for m2 in self.order:
				if m2[0] == m2_no:

					if m2[3] == 'seq_cst':
						for j in range(len(self.order)):
							b = self.order[j]
							if type(b) is list and b[6] == m1_no:
								x = self.order[j-1]

								if not (x,m2_no) in self.to_edges:
									self.to_edges.append((x,m2_no))

	def rule3_1b(self):
		for i in self.mo_edges:
			m_no = str(i[0])
			a_no = str(i[1])
			x = 0
			y = []														# since there might be multiple read instructions with rf m_no
			b = []
			seq = 0

			for j in range(len(self.order)):
				if type(self.order[j]) is list and self.order[j][6] == m_no:
					y.append(self.order[j-1])

					# checking also for rule 1b, if B is sequential
					if self.order[j][3] == 'seq_cst':
						b.append(self.order[j][0])

				if self.order[j][0] == a_no:
					x = self.order[j+1]

					# checking also for rule 1b, if A is seq_cst
					if self.order[j][3] == 'seq_cst':
						seq += 1

			# rule 1b
			if seq == 1 and b:
				for b_no in b:
					if not (b_no,a_no) in self.to_edges:
						self.to_edges.append((b_no,a_no))

			# rule 3
			if x and y:
				for fy in y:
					if not (fy,x) in self.to_edges:
						self.to_edges.append((fy,x))

	def rule4(self):
		for i in self.mo_edges:
			b_no = i[0]
			a_no = i[1]

			for j in range(len(self.order)):
				if self.order[j][0] == b_no:
					b = self.order[j]
					y = self.order[j-1]
				if self.order[j][0] == a_no:
					a = self.order[j]
					x = self.order[j+1]

			# rule 4a
			if b[3] == 'seq_cst':
				if not (b_no,x) in self.to_edges:
					self.to_edges.append((b_no,x))

			# rule 4b
			if a[3] == 'seq_cst':
				if not (y,a_no) in self.to_edges:
					self.to_edges.append((y,a_no))

			# rule 4c
			if not (y,x) in self.to_edges:
				self.to_edges.append((y,x))

	# unused
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
	
	# unused
	def create_transitive_edges(self):
		to_edges_basic_store = open("store/to_edges_basic_store",'r')
		to_edges = to_edges_basic_store.read()
		to_edges_basic_store.close()

		to_edges = ast.literal_eval(to_edges)
