# --------------------------------------------------------
#
# --------------------------------------------------------

import ast
import networkx as nx
from constants import *

class to:

	def __init__(self, order, reads, writes, fences_thread, mo_edges, to_edges):
		self.order = order
		self.fences_thread = fences_thread
		self.mo_edges = mo_edges
		self.to_edges = to_edges
		self.writes = writes					# writes with their before and after fences
		self.reads = reads						# reads with their before and after fences

		self.rule0()
		self.rule1()
		self.rule2()
		self.rule3()
		self.rule4_1()
		self.rule4_2()
		self.rule5()

		self.to_edges = list(set(self.to_edges))		# remove duplicates

	def get(self):
		return self.to_edges
	
	def rule0(self):
		for b in range(len(self.reads)):
			if type(self.reads[b]) is list:
				try: a = next(i for i,v in enumerate(self.writes) 
								if type(v) is list and v[S_NO] == self.reads[b][RF]) 	# the write which sends the rf value to b
				except: continue

				# finding indexes of places to stop
				x_thread = self.writes[a][T_NO] -1
				x0 = self.fences_thread[x_thread][0]
				x0_index = self.order.index(x0)
				a_index = self.order.index(self.writes[a])
				
				# adding relations for all sc events above A with all sc events below B
				for i in range(x0_index, a_index):
					a_sb = self.order[i]
					if type(a_sb) is list and a_sb[TYPE] == WRITE and a_sb[MO] == SEQ_CST:
						x = a_sb[S_NO]
					elif type(a_sb) is not list:
						x = a_sb
					else: continue

					if self.reads[b][MO] == SEQ_CST:
						self.to_edges.append((x,self.reads[b][S_NO]))

					y = self.reads[b+1]
					y_thread = self.reads[b][T_NO] -1
					y_index = self.fences_thread[y_thread].index(y)

					for i in range(y_index, len(self.fences_thread[y_thread])):
						self.to_edges.append((x, self.fences_thread[y_thread][i]))

	def rule1(self):
		for b in self.reads:
			if type(b) is list and b[MO] == SEQ_CST:
				try: a = next(i for i,v in enumerate(self.writes)
								if type(v) is list and v[S_NO] == b[RF])
				except: continue

				# rule 1a = sohb, somo, sorf
				if self.writes[a][MO] == SEQ_CST:
					self.to_edges.append((self.writes[a][S_NO],b[S_NO]))
				# rule 1b = sofr
				for m in self.mo_edges:
					if self.writes[a][S_NO] == m[0]:
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

			# all RF's from m[0]
			b_s = []
			try:
				b_generator = (i for i,v in enumerate(self.reads) 
					if type(v) is list and v[RF] == m[0])
				for i in b_generator:
					b_s.append(i)
			except: continue

			for b in b_s:
				# rule 2a = soFWfr
				if self.writes[m2][MO] == SEQ_CST:
					x = self.reads[b-1]
					m2_no = self.writes[m2][S_NO]

					# adding relations for fences above B
					x_thread = self.reads[b][T_NO] -1
					x_index = self.fences_thread[x_thread].index(x)

					for i in range(0, x_index+1):
						self.to_edges.append((self.fences_thread[x_thread][i], m2_no))

				# rule 2b = soRFfr
				if self.reads[b][MO] == SEQ_CST:
					y = self.writes[m2-1]
					b_no = self.reads[b][S_NO]

					# adding relations for fences above M2
					y_thread = self.writes[m2][T_NO] -1
					y_index = self.fences_thread[y_thread].index(y)

					for i in range(0, y_index+1):
						self.to_edges.append((b_no, self.fences_thread[y_thread][i]))

	def rule3(self): # - soFFfr
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

	def rule4_1(self):
		# for the mo relations
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

			# rule 4a = soEF
			if self.writes[b_index][MO] == SEQ_CST:
				for i in range(x_index,len(self.fences_thread[x_thread])): # all fences below A ( B -sb-> X )
					self.to_edges.append((b, self.fences_thread[x_thread][i]))
			# rule 4b = soFE
			if self.writes[a_index][MO] == SEQ_CST:
				for i in range(0, y_index+1):
					self.to_edges.append((self.fences_thread[y_thread][i], a)) # all fences above B ( Y -sb-> A )
			# rule 4c = soFF
			for i in range(0, y_index+1):
				for j in range(x_index, len(self.fences_thread[x_thread])):
					self.to_edges.append((self.fences_thread[y_thread][i],self.fences_thread[x_thread][j])) # all fences below A and above B
		
	def rule4_2(self):
		# for the rf relations
		for b_index in range(len(self.writes)):
			b = self.writes[b_index]
			if type(b) is list:
				a_indexes = []
				try: 
					a_generator = (i for i,v in enumerate(self.reads) 
						if type(v) is list and v[RF] == b[S_NO])
					for i in a_generator:
						a_indexes.append(i)
				except: continue

				for a_index in a_indexes:
					x = self.reads[a_index+1]
					y = self.writes[b_index-1]
					
					# for all fence relations
					x_thread = self.reads[a_index][T_NO] -1
					x_index = self.fences_thread[x_thread].index(x)
					y_thread = self.writes[b_index][T_NO] -1
					y_index = self.fences_thread[y_thread].index(y)

					# rule 4a = soEF
					if b[MO] == SEQ_CST:
						for i in range(x_index,len(self.fences_thread[x_thread])): # all fences below A ( B -sb-> X )
							self.to_edges.append((b[S_NO], self.fences_thread[x_thread][i]))
					# rule 4b = soFE
					if self.reads[a_index][MO] == SEQ_CST:
						for i in range(0, y_index+1):
							self.to_edges.append((self.fences_thread[y_thread][i], self.reads[a_index][S_NO])) # all fences above B ( Y -sb-> A )
					# rule 4c = soFF
					for i in range(0, y_index+1):
						for j in range(x_index, len(self.fences_thread[x_thread])):
							self.to_edges.append((self.fences_thread[y_thread][i],self.fences_thread[x_thread][j])) # all fences below A and above B
	
	def rule5(self):
		for m in self.mo_edges:
			try: m2_index = next(i for i,v in enumerate(self.writes) 
						if type(v) is list and v[S_NO] == m[1])
			except: continue
			m2 = self.writes[m2_index]

			# getting all RF's from m2
			b2_indexes = []
			try: 
				b2_generator = (i for i,v in enumerate(self.reads) 
					if type(v) is list and v[RF] == m[0])
				for i in b2_generator:
					b2_indexes.append(i)
			except: continue

			for b2_index in b2_indexes:
				b2 = self.reads[b2_index]

				# rule 5a
				for i in range(b2_index):
					read = self.reads[i]
					# if (it is a read) and (it is in sb with b2) and (it is an sc read)
					if type(read) is list and read[T_NO] == b2[T_NO] and read[MO] == SEQ_CST:
						b1_index = read[S_NO]

						# adding relations for fences below m2
						y = self.writes[m2_index+1]
						y_thread = self.writes[m2_index][T_NO] -1
						y_index = self.fences_thread[y_thread].index(y)
						for i in range(y_index,len(self.fences_thread[y_thread])):
							self.to_edges.append((b1_index, self.fences_thread[y_thread][i]))

				# rule 5b
				for i in range(m2_index, len(self.writes)):
					write = self.writes[i]
					# if it has gone beyond the thread, then it is not in sb anymore
					if type(write) is list and write[T_NO] != m2[T_NO]:
						break
					else:
						if type(write) is list and write[MO] == SEQ_CST:
							b1_index = write[S_NO]

							# adding relations for fences above b2
							x = self.reads[b2_index-1]
							x_thread = self.reads[b2_index][T_NO] -1
							x_index = self.fences_thread[x_thread].index(x)
							for i in range(0, x_index+1):
								self.to_edges.append((self.fences_thread[x_thread][i], b1_index))

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
