# --------------------------------------------------------
#
# --------------------------------------------------------

import ast
from graph import Graph
import networkx as nx
from constants import *

class to:

	def __init__(self,order,fences_thread,mo_edges,to_edges):
		self.order = order
		self.fences_thread = fences_thread
		self.mo_edges = mo_edges
		self.to_edges = to_edges

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
			if order[i][TYPE] == WRITE or order[i][TYPE] == RMW:
				self.writes.append(order[i-1])
				self.writes.append(order[i])
				self.writes.append(order[i+1])
			if order[i][TYPE] == READ or order[i][TYPE] == RMW:
				self.reads.append(order[i-1])
				self.reads.append(order[i])
				self.reads.append(order[i+1])

	def rule0(self):
		for b in range(len(self.reads)):
			if type(self.reads[b]) is list:
				y = self.reads[b+1]
				try: a = next(i for i,v in enumerate(self.writes) 
								if type(v) is list and v[S_NO] == self.reads[b][RF]) 	# the write which send the rf value to b
				except: continue

				x = self.writes[a-1]
				
				# adding relations for fences above A and fences below B
				x_thread = self.writes[a][T_NO] -1
				y_thread = self.reads[b][T_NO] -1
				x_index = self.fences_thread[x_thread].index(x)
				y_index = self.fences_thread[y_thread].index(y)

				for i in range(0, x_index+1):
					for j in range(y_index, len(self.fences_thread[y_thread])):
						self.to_edges.append((self.fences_thread[x_thread][i],self.fences_thread[y_thread][j]))
	
	def rule1(self):
		for b in self.reads:
			if type(b) is list and b[MO] == SEQ_CST:
				try: a = next(i for i,v in enumerate(self.writes)
								if type(v) is list and v[S_NO] == b[RF])
				except: continue

				# rule 1a
				if self.writes[a][MO] == SEQ_CST:
					self.to_edges.append((self.writes[a][S_NO],b[S_NO]))
				# rule 1b
				for m in self.mo_edges:
					if m[0] == self.writes[a][S_NO]:
						try: m1 = next(i for i,v in enumerate(self.writes)
								if type(v) is list and v[S_NO] == m[1])
						except: continue

						if self.writes[m1][MO] == SEQ_CST:
							self.to_edges.append((b[S_NO],self.writes[m1][S_NO]))
	
	def rule2(self):
		for m in self.mo_edges:
			try: m2 = next(i for i,v in enumerate(self.writes) 
						if type(v) is list and v[S_NO] == m[1])
			except: continue

			if self.writes[m2][MO] == SEQ_CST:
				try: b = next(i for i,v in enumerate(self.reads) 
							if type(v) is list and v[RF] == m[0])
				except: continue

				x = self.reads[b-1]
				m2_no = self.writes[m2][S_NO]

				# adding relations for fences above B
				x_thread = self.reads[b][T_NO] -1
				x_index = self.fences_thread[x_thread].index(x)

				for i in range(0, x_index+1):
					self.to_edges.append((self.fences_thread[x_thread][i], m2_no))

	def rule3(self):
		for b in range(len(self.reads)):
			if type(self.reads[b]) is list:
				b_rf = self.reads[b][RF]
				for m in self.mo_edges:
					if m[0] == b_rf:
						a = m[1]
						try: a_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == a)
						except: continue
						
						x = self.writes[a_index+1]
						y = self.reads[b-1]

						# adding relations for all fences below A and fences above B
						x_thread = self.writes[a_index][T_NO] -1
						y_thread = self.reads[b][T_NO] -1
						x_index = self.fences_thread[x_thread].index(x)
						y_index = self.fences_thread[y_thread].index(y)

						for i in range(0, y_index+1):
							for j in range(x_index, len(self.fences_thread[x_thread])):
								self.to_edges.append((self.fences_thread[y_thread][i],self.fences_thread[x_thread][j]))

	def rule4(self):
		for m in self.mo_edges:
			b = m[0]
			a = m[1]
			try:
				b_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == b)
				a_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == a)
			except: continue
			
			x = self.writes[a_index+1]
			y = self.writes[b_index-1]
			
			# for all fence relations
			x_thread = self.writes[a_index][T_NO] -1
			x_index = self.fences_thread[x_thread].index(x)
			y_thread = self.writes[b_index][T_NO] -1
			y_index = self.fences_thread[y_thread].index(y)

			# rule 4a
			if self.writes[b_index][MO] == SEQ_CST:
				for i in range(x_index,len(self.fences_thread[x_thread])):
					self.to_edges.append((b, self.fences_thread[x_thread][i]))
			# rule 4b
			if self.writes[a_index][MO] == SEQ_CST:
				for i in range(0, y_index+1):
					self.to_edges.append((self.fences_thread[y_thread][i], a))
			# rule 4c
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
