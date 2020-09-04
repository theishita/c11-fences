# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class mo:

	def __init__(self,trace,mat,size):
		self.mat = mat
		self.size = size

		self.writes = []
		self.reads = []
		self.mo_edges = []                      								# list of mo edges

		self.preprocessing(trace)

		self.rule1()
		self.rule2()
		self.rule3()
		self.rule4()

		self.mo_edges = list(dict.fromkeys(self.mo_edges))
		# print("mo edges=",self.mo_edges)

	def get(self):
		return self.mo_edges

	def preprocessing(self, trace):
		for t in trace:
			if t[2] == "write" or t[2] == "init" or t[2] == "rmw":
				self.writes.append(t)
			if t[2] == "read" or t[2] == "rmw":
				self.reads.append(t)
	
	def rule1(self):
		for a in self.writes:
			for b in self.writes:
				if a[4] == b[4] and self.mat.containsEdge(a[0],b[0]):		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((a[0],b[0]))
	
	def rule2(self):
		for a in self.reads:
			x = a[6]
			for b in self.reads:
				if a[4] == b[4] and b[6] != x and self.mat.containsEdge(a[0],b[0]): 	# checking if variable operated upon is same and if there is hb
					y = b[6]
					self.mo_edges.append((x,y))

	def rule3(self):
		for a in self.reads:
			x = a[6]
			for b in self.reads:
				if b[4] == a[4] and self.mat.containsEdge(a[0],b[0]): 		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((x,b[0]))
	
	def rule4(self):
		for x in self.writes:
			for b in self.reads:
				if x[4] == b[4] and self.mat.containsEdge(x[0],b[0]) and b[6] != x[0]:
					y = b[6]
					self.mo_edges.append((x[0],y))

	# unused
	# to get all the transitive mo relations as well
	def all_mo(self):
		flag = 0
		temp_edges = self.mo_edges

		while flag != 2:
			for e0 in self.mo_edges:
				for e1 in self.mo_edges:
					if e0[1] == e1[0] and (e0[0],e1[1]) not in self.mo_edges:
						self.mo_edges.append((e0[0],e1[1]))


			if self.mo_edges == temp_edges:
				flag += 1
			else:
				temp_edges = self.mo_edges