# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class hb:

	def __init__(self,trace):

		self.sb_edges = []									# list of all sb edges between instructions
		self.sw_edges = []									# list of all sw edges between instructions
		self.to_edges = []									# list of to edges between instructions
		self.size = 0										# number of instructions in the execution trace

		threads = int(trace[-1][1])							# find out the number of total threads in the program

		self.sb(trace,threads)
		self.sw(trace,threads)

		self.size += 1
		self.mat = Graph(self.size)
		self.matrix()

	def get(self):
		return self.mat,self.size,self.to_edges

	def matrix(self):

		# loop for basic sb edges
		for i in range(len(self.sb_edges)):
			v1 = int(self.sb_edges[i][0])
			v2 = int(self.sb_edges[i][1])
			self.mat.addEdge(v1,v2)

		# loop for basic sw edges
		for i in range(len(self.sw_edges)):
			v1 = int(self.sw_edges[i][0])
			v2 = int(self.sw_edges[i][1])
			self.mat.addEdge(v1,v2)

		temp = Graph(self.size)
		temp.adjMatrix = self.mat.adjMatrix
		flag = 0

		while flag!=2:
			for i in range(self.size):
				v1 = i

				for j in range(self.size):
					if(self.mat.containsEdge(v1,j)):
						for k in range(self.size):
							if(self.mat.containsEdge(j,k)):
								self.mat.addEdge(v1,k)

			if(temp.adjMatrix == self.mat.adjMatrix):
				flag += 1

			temp.adjMatrix = self.mat.adjMatrix

	def sb(self,trace,threads):

		# getting a list of sb as tuples of two
		for i in range(len(trace)-1):
			if trace[i][1] == trace[i+1][1]:
				self.sb_edges.append((trace[i][0],trace[i+1][0]))
				
				if int(trace[i+1][0]) > self.size:
					self.size = int(trace[i+1][0])

	def sw(self,trace,threads):

		write_models = ["release",'acq_rel',"seq_cst"]
		read_models = ["acquire",'acq_rel',"seq_cst"]

		for i in range(len(trace)):
			# create sw's between thread create statements
			if trace[i][2] == "create":
				v1 = trace[i][0]
				v2 = str(int(v1)+1)
				self.sw_edges.append((v1,v2))
				self.to_edges.append((v1,v2))


			# create sw's between thread finish and join statements
			if trace[i][2] == "join":
				t = trace[i][5]			# value/number of the thread which is getting joined
				for j in range(i,len(trace)):
					if trace[j][2] == "finish" and trace[j][1] == t:
						v1 = trace[j][0]
						v2 = trace[i][0]
						self.sw_edges.append((v1,v2))
						self.to_edges.append((v1,v2))

			# create sw's between read/rmw and write/rmw statements
			if trace[i][2] == 'read' or trace[i][2] == 'rmw':
				rf = trace[i][6]
				for j in trace:
					if j[0] == rf:
						if j[3] in write_models and trace[i][3] in read_models:
							self.sw_edges.append((j[0],trace[i][0]))
							if j[3] == 'seq_cst' and trace[i][3] == 'seq_cst':
								self.to_edges.append((j[0],trace[i][0]))
