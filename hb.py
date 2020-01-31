# --------------------------------------------------------

# --------------------------------------------------------

class Graph:
    def __init__(self, size):
        self.adjMatrix = []
        for i in range(size):
            self.adjMatrix.append([0 for i in range(size)])
        self.size = size
    def addEdge(self, v1, v2):
        if v1 == v2:
            print("Same vertex %d and %d" % (v1, v2))
        self.adjMatrix[v1][v2] = 1
    def removeEdge(self, v1, v2):
        if self.adjMatrix[v1][v2] == 0:
            print("No edge between %d and %d" % (v1, v2))
            return
        self.adjMatrix[v1][v2] = 0
    def containsEdge(self, v1, v2):
        return True if self.adjMatrix[v1][v2] > 0 else False
    def __len__(self):
        return self.size
        
    def toString(self):
        for row in self.adjMatrix:
            print(row)

class hb:
	
	def __init__(self,trace):

		self.sb_edges = []							# list of all sb edges between instructions
		self.sw_edges = []							# list of all sw edges between instructions
		self.size = 0								# number of instructions in the execution trace
		self.edge_map = {}							# instruction numbers mapped to graph indices

		# find out the number of threads in the program
		threads = 0
		for a in trace:
			threads = max(threads,int(a[1]))
		
		self.sb(trace,threads)
		self.sw(trace,threads)

		mat = Graph(self.size)
		flag = 0

		# loop for basic sb edges
		for i in range(len(self.sb_edges)):
			v1 = self.edge_map[self.sb_edges[i][0]]
			v2 = self.edge_map[self.sb_edges[i][1]]
			mat.addEdge(v1,v2)

		while flag==0:
			temp = Graph(self.size)
			temp.adjMatrix = mat.adjMatrix

			for i in range(len(self.sb_edges)):
				v1 = self.edge_map[self.sb_edges[i][0]]

				for j in range(self.size):
					if(mat.containsEdge(v1,j)):
						for k in range(self.size):
							if(mat.containsEdge(j,k)):
								mat.addEdge(v1,k)
			
			if(temp.adjMatrix == mat.adjMatrix):
				flag = 1
			
		
		# mat.toString()
		# self.hb_matrix(self.sb_edges)
		# print(self.sb_edges,self.edge_map)
		
	def sb(self,trace,threads):

		key = 0
		for j in range(2,threads+1):
			sb_order = []
			for i in range(len(trace)):
				# print(trace[i])
				if int(trace[i][1]) == j:
					sb_order.append(trace[i][0])
					self.size += 1
					self.edge_map[trace[i][0]] = key
					key += 1
			sb_tuples = []

			# converting the list of sb into tuples of two
			for i in range(len(sb_order)):
				if not i == len(sb_order)-1:
					sb_tuples.append((sb_order[i],sb_order[i+1]))
			
			for i in sb_tuples:
				self.sb_edges.append(i)

	def sw(self,trace,threads):
		instr = []
		for i in trace:
			if i[3] == "write":
				inf = {}
				inf['no'] = i[0]
				inf['type'] = 'write'
				inf['model'] = i[4]
				instr.append(inf)
			if i[3] == "read":
				inf = {}
				inf['no'] = i[0]
				inf['type'] = 'read'
				inf['model'] = i[4]
				inf['rf'] = i[7]
				instr.append(inf)
		
		print(instr)