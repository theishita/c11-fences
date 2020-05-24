# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class to:

	def __init__(self,order,mo_edges,sb_edges,order_thread):
		self.order = order
		self.order_thread = order_thread
		self.mo_edges = mo_edges
		self.sb_edges = sb_edges

		# print("sb_edges=",sb_edges)

		self.to_edges = sb_edges

		self.rule0()
		self.rule1a()
		self.rule2()
		self.rule3_1b()
		self.rule4()

		# sort and remove duplicates
		self.to_edges = list(set(self.to_edges))
		self.to_edges.sort(key = lambda x: x[0])

		self.all_to()
		self.to_edges = list(set(self.to_edges))
		self.to_edges.sort(key = lambda x: x[0])

		# print("to edges=",self.to_edges)
		# print("to edges=")
		#
		# for e in self.to_edges:
		# 	if 'F' not in e[0] and 'F' not in e[1]:
		# 		print(e)

	def get(self):
		return self.to_edges

	def rule0(self):
		# print("\n\nTO rule 0:")
		for i in range(len(self.order)):
			b = self.order[i]
			if "type" in b and (b['type'] == 'read' or b['type'] == 'rmw'):
				a_no = b['rf']
				y_thread = b['thread'] - 1

				y = self.order[i+1]

				for j in range(len(self.order)):
					a = self.order[j]
					if 'no' in a and a['no'] == a_no:
						x = self.order[j-1]
						x_thread = a['thread'] - 1

						self.to_edges.append((x,y))
						# print("(",x,",",y,")")

	def rule1a(self):
		# print("\n\nTO rule 1a:")
		for b in self.order:
			if 'type' in b and (b['type'] == 'read' or b['type'] == 'rmw') and b['mo'] == 'seq_cst':
				b_no = b['no']
				b_thread = b['thread'] - 1

				a_no = b['rf']

				for a in self.order:
					if 'no' in a and a['no'] == a_no and a['mo'] == 'seq_cst':
						a_thread = a['thread'] - 1

						self.to_edges.append((a_no,b_no))
						# print("(",a_no,",",b_no,")")

	def rule2(self):
		# print("\n\nTO rule 2:")
		for i in self.mo_edges:
			m1_no = str(i[0])
			m2_no = str(i[1])

			for m2 in self.order:
				if 'type' in m2 and m2['no'] == m2_no:
					m2_thread = m2['thread'] - 1

					if m2['mo'] == 'seq_cst':
						for j in range(len(self.order)):
							b = self.order[j]
							if 'rf' in b and b['rf'] == m1_no:
								x_thread = b['thread'] - 1
								x = self.order[j-1]

								self.to_edges.append((x,m2_no))
								# print("(",x,",",m2_no,")")

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
			# print("\n\nTO rule 1b:")
			if seq == 1 and b:
				for b_no in b:
					self.to_edges.append((b_no,a_no))
					# print("(",b_no,",",a_no,")")

			# rule 3
			if x and y:
				for fy in y:
					self.to_edges.append((fy,x))
					# print("(",fy,",",x,")")

	def rule4(self):
		for i in self.mo_edges:
			b_no = i[0]
			a_no = i[1]

			for j in range(len(self.order)):
				if 'type' in self.order[j] and self.order[j]['no'] == b_no:
					b = self.order[j]
					b_thread = self.order[j]['thread'] - 1
					y = self.order[j-1]
				if 'type' in self.order[j] and self.order[j]['no'] == a_no:
					a = self.order[j]
					a_thread = self.order[j]['thread'] - 1
					x = self.order[j+1]

			# rule 4a
			# print("\n\nTO rule 4a:")
			if b['mo'] == 'seq_cst':
				self.to_edges.append((b_no,x))
				# print("(",b_no,",",x,")")

			# rule 4b
			# print("\n\nTO rule 4b:")
			if a['mo'] == 'seq_cst':
				self.to_edges.append((y,a_no))
				# print("(",y,",",a_no,")")

			# rule 4c
			# print("\n\nTO rule 4c:")
			self.to_edges.append((y,x))
			# print("(",y,",",x,")")

	def all_to(self):
		flag = 0
		temp_edges = self.to_edges
		while flag != 2:
			for edge in self.to_edges:
				e1 = edge[0]
				e2 = edge[1]

				for sb in self.sb_edges:
					s1 = sb[0]
					if e2 == s1:
						s2 = sb[1]
						if (e1,s2) not in self.to_edges:
							self.to_edges.append((e1,s2))

				for sb in self.sb_edges:
					s2 = sb[1]
					if e1 == s2:
						s1 = sb[0]
						if (s1,e2) not in self.to_edges:
							self.to_edges.append((s1,e2))

			if self.to_edges == temp_edges:
				flag += 1
			else:
				temp_edges = self.to_edges
