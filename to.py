# --------------------------------------------------------

# --------------------------------------------------------

class to:

	def __init__(self,order,mo_edges,sb_edges):
		self.order = order
		self.mo_edges = mo_edges
		self.sb_edges = sb_edges

		self.to_edges = []

		self.rule1()
		self.rule2()
		self.rule3()

		self.all_to()

		self.to_edges.sort(key = lambda x: x[0])

		# print("to edges=",self.to_edges)

	def get(self):
		return self.to_edges

	def rule1(self):
		for i in range(len(self.order)):
			if "type" in self.order[i] and (self.order[i]['type'] == 'read' or self.order[i]['type'] == 'rmw'):
				a_no = self.order[i]['rf']

				y = self.order[i+1]

				for j in range(len(self.order)):
					if 'no' in self.order[j] and self.order[j]['no'] == a_no:
						x = self.order[j-1]
						self.to_edges.append((x,y))

	def rule2(self):
		for i in self.mo_edges:
			m = i[0]
			a = i[1]
			x = 0
			y = 0

			for j in range(len(self.order)):
				b = self.order[j]
				if "type" in b and (b['type'] == 'read' or b['type'] == 'rmw'):
					if b['rf'] == m:
						# print("bma=",b,m,a)
						y = self.order[j-1]
				if 'no' in self.order[j] and self.order[j]['no'] == a:
					x = self.order[j+1]

			if x and y:
				self.to_edges.append((y,x))

	def rule3(self):
		for i in self.mo_edges:
			b = i[0]
			a = i[1]
			x = 0
			y = 0

			for j in range(len(self.order)):
				if 'no' in self.order[j] and self.order[j]['no'] == b:
					y = self.order[j-1]
				if 'no' in self.order[j] and self.order[j]['no'] == a:
					x = self.order[j+1]

			if x and y:
				self.to_edges.append((y,x))

	def all_to(self):
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
					for to in self.to_edges:
						t1 = to[0]
						t2 = to[1]
						if t1 == s2 and (s1,t2) not in self.to_edges:
							self.to_edges.append((s1,t2))
