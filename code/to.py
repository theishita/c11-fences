# --------------------------------------------------------

# --------------------------------------------------------

import ast
from graph import Graph
import networkx as nx
from constants import *

class to:

	def __init__(self,order,fences_thread,mo_edges,sb_edges,to_edges):
		self.order = order
		self.fences_thread = fences_thread
		self.mo_edges = mo_edges
		self.sb_edges = sb_edges

		self.to_edges = to_edges + sb_edges

		self.writes = []					# writes with their before and after fences
		self.reads = []						# reads with their before and after fences
		self.preprocessing(order)

		self.rule0()
		self.rule1()
		self.rule2()
		self.rule3()
		self.rule4()

		self.to_edges = list(set(self.to_edges))		# remove repeats

	def get(self):
		return self.to_edges
	
	def preprocessing(self, order):
		for i in range(len(order)):
			if order[i][2] == WRITE or order[i][2] == RMW or order[i][2] == INIT: # TODO: is init to be included here as well???
				self.writes.append(order[i-1])
				self.writes.append(order[i])
				self.writes.append(order[i+1])
			if order[i][2] == READ or order[i][2] == RMW:
				self.reads.append(order[i-1])
				self.reads.append(order[i])
				self.reads.append(order[i+1])

	def rule0(self):
		for b in range(len(self.reads)):
			if type(self.reads[b]) is list:
				y = self.reads[b+1]
				a = next(i for i,v in enumerate(self.writes) 
							if type(v) is list and v[0] == self.reads[b][6]) 	# the write which send the rf value to b
				x = self.writes[a-1]
				# self.to_edges.append((x,y))
				
				# adding relations for fences above A and fences below B
				x_thread = self.writes[a][1] -1
				y_thread = self.reads[b][1] -1
				x_index = self.fences_thread[x_thread].index(x)
				y_index = self.fences_thread[y_thread].index(y)

				for i in range(0, x_index+1):
					for j in range(y_index, len(self.fences_thread[y_thread])):
						self.to_edges.append((self.fences_thread[x_thread][i],self.fences_thread[y_thread][j]))
	
	def rule1(self):
		for b in self.reads:
			if type(b) is list and b[3] == SEQ_CST:
				a = next(i for i,v in enumerate(self.writes)
							if type(v) is list and v[0] == b[6])
				
				# rule 1a
				if self.writes[a][3] == SEQ_CST:
					self.to_edges.append((self.writes[a][0],b[0]))
				# rule 1b
				for m in self.mo_edges:
					if m[0] == self.writes[a][0]:
						m1 = next(i for i,v in enumerate(self.writes)
								if type(v) is list and v[0] == m[1])
						if self.writes[m1][3] == SEQ_CST:
							self.to_edges.append((b[0],self.writes[m1][0]))
	
	def rule2(self):
		for m in self.mo_edges:
			m2 = next(i for i,v in enumerate(self.writes) 
						if type(v) is list and v[0] == m[1])
			if self.writes[m2][3] == SEQ_CST:
				try:
					b = next(i for i,v in enumerate(self.reads) 
							if type(v) is list and v[6] == m[0])
					x = self.reads[b-1]
					# self.to_edges.append((x, self.writes[m2][0]))
					m2_no = self.writes[m2][0]

					# adding relations for fences above B
					x_thread = self.reads[b][1] -1
					x_index = self.fences_thread[x_thread].index(x)

					for i in range(0, x_index+1):
						self.to_edges.append((self.fences_thread[x_thread][i], m2_no))

				except:
					continue

	def rule3(self):
		for b in range(len(self.reads)):
			if type(self.reads[b]) is list:
				b_rf = self.reads[b][6]
				for m in self.mo_edges:
					if m[0] == b_rf:
						a = m[1]
						a_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[0] == a)
						x = self.writes[a_index+1]
						y = self.reads[b-1]
						# self.to_edges.append((y,x))

						# adding relations for all fences below A and fences above B
						x_thread = self.writes[a_index][1] -1
						y_thread = self.reads[b][1] -1
						x_index = self.fences_thread[x_thread].index(x)
						y_index = self.fences_thread[y_thread].index(y)

						for i in range(0, y_index+1):
							for j in range(x_index, len(self.fences_thread[x_thread])):
								self.to_edges.append((self.fences_thread[y_thread][i],self.fences_thread[x_thread][j]))

	def rule4(self):
		for m in self.mo_edges:
			b = m[0]
			a = m[1]
			b_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[0] == b)
			a_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[0] == a)
			x = self.writes[a_index+1]
			y = self.writes[b_index-1]
			
			# for all fence relations
			x_thread = self.writes[a_index][1] -1
			x_index = self.fences_thread[x_thread].index(x)
			y_thread = self.writes[b_index][1] -1
			y_index = self.fences_thread[y_thread].index(y)

			# rule 4a
			if self.writes[b_index][3] == SEQ_CST:
				# self.to_edges.append((b,x))
				for i in range(x_index,len(self.fences_thread[x_thread])):
					self.to_edges.append((b, self.fences_thread[x_thread][i]))
			# rule 4b
			if self.writes[a_index][3] == SEQ_CST:
				# self.to_edges.append((y,a))
				for i in range(0, y_index+1):
					self.to_edges.append((self.fences_thread[y_thread][i], a))
			# rule 4c
			# self.to_edges.append((y,x))
			for i in range(0, y_index+1):
				for j in range(x_index, len(self.fences_thread[x_thread])):
					self.to_edges.append((self.fences_thread[y_thread][i],self.fences_thread[x_thread][j]))

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
