# --------------------------------------------------------

# --------------------------------------------------------

class to:

	def __init__(self,order,mo_edges):
		self.order = order
		self.mo_edges = mo_edges

		self.to_edges = []

		self.rule1()
		self.rule2()
		self.rule3()

		print("to edges=",self.to_edges)
	
	def get(self):
		return self.to_edges

	def rule1(self):
		for i in range(len(self.order)):
			if "type" in self.order[i] and self.order[i]['type'] == 'read':
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
				if "type" in b and b['type'] == 'read':
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