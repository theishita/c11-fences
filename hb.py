# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class hb:

	mat = Graph(0)
	vertex_map = None
	
	def __init__(self,trace):

		self.sb_edges = []									# list of all sb edges between instructions
		self.sw_edges = []									# list of all sw edges between instructions
		self.size = 0										# number of instructions in the execution trace
		self.vertex_map = {}								# instruction numbers mapped to graph indices
		self.instr = []										# properties of each instruction

		# find out the number of threads in the program
		threads = 0
		for a in trace:
			threads = max(threads,int(a[1]))
		
		self.sb(trace,threads)
		self.sw(trace,threads)
		
		self.mat = Graph(self.size)
		self.matrix()

	def matrix(self):

		flag = 0

		# loop for basic sb edges
		for i in range(len(self.sb_edges)):
			v1 = self.vertex_map[self.sb_edges[i][0]]
			v2 = self.vertex_map[self.sb_edges[i][1]]
			self.mat.addEdge(v1,v2)
		
		# loop for basic sw edges
		for i in range(len(self.sw_edges)):
			v1 = self.vertex_map[self.sw_edges[i][0]]
			v2 = self.vertex_map[self.sw_edges[i][1]]
			self.mat.addEdge(v1,v2)

		while flag==0:
			temp = Graph(self.size)
			temp.adjMatrix = self.mat.adjMatrix

			for i in range(len(self.sb_edges)):
				v1 = self.vertex_map[self.sb_edges[i][0]]

				for j in range(self.size):
					if(self.mat.containsEdge(v1,j)):
						for k in range(self.size):
							if(self.mat.containsEdge(j,k)):
								self.mat.addEdge(v1,k)
			
			if(temp.adjMatrix == self.mat.adjMatrix):
				flag = 1
			
		
		self.mat.toString()
		# self.hb_matrix(self.sb_edges)
		print(self.sb_edges,self.vertex_map)
		print(self.sw_edges)

		
	def sb(self,trace,threads):

		key = 0
		for j in range(threads+1):
			sb_order = []
			for i in range(len(trace)):
				# print(trace[i])
				if int(trace[i][1]) == j:
					sb_order.append(trace[i][0])
					self.size += 1
					self.vertex_map[trace[i][0]] = key
					key += 1
			sb_tuples = []

			# converting the list of sb into tuples of two
			for i in range(len(sb_order)):
				if not i == len(sb_order)-1:
					sb_tuples.append((sb_order[i],sb_order[i+1]))
			
			for i in sb_tuples:
				self.sb_edges.append(i)

	def sw(self,trace,threads):
		# print(trace)
		write_models = ["release","seq_cst"]
		read_models = ["acquire","seq_cst"]
		for i in range(len(trace)):
			if trace[i][3] == "create":														# sw's between thread create statements
				v1 = trace[i][0]
				v2 = trace[i+1][0]
				self.sw_edges.append((v1,v2))
			if trace[i][3] == "finish":														# sw's between thread finish and join statements
				t = '0x'+trace[i][1]
				print("t=",t)
				for j in range(i,len(trace)):
					if trace[j][3] == "join" and trace[j][6] == t:
						v1 = trace[i][0]
						v2 = trace[j][0]
						self.sw_edges.append((v1,v2))
			if trace[i][3] == "write":
				inf = {}
				inf['no'] = trace[i][0]
				inf['type'] = 'write'
				inf['model'] = trace[i][4]
				inf['loc'] = trace[i][5]
				self.instr.append(inf)
			if trace[i][3] == "read":
				inf = {}
				inf['no'] = trace[i][0]
				inf['type'] = 'read'
				inf['model'] = trace[i][4]
				inf['loc'] = trace[i][5]
				inf['rf'] = trace[i][7]
				self.instr.append(inf)
			if trace[i][2] == "init":
				inf = {}
				inf['no'] = trace[i][0]
				inf['type'] = 'write'
				inf['model'] = trace[i][4]
				inf['loc'] = trace[i][5]
				self.instr.append(inf)
		
		for i in self.instr:
			if i['type'] == 'read':
				rf = i['rf']
				for j in self.instr:
					if j['no'] == rf:
						if j['model'] in write_models and i['model'] in read_models:
							self.sw_edges.append((j['no'],i['no']))

	def get(self):
		return self.mat,self.vertex_map,self.instr,self.size
		
		