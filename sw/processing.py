from sw import sw

class Processing:
	def __init__(self,p,filename):

		print(filename)

		self.traces = []											# lists of all execution traces
		self.events_order = []										# order of events including fences
		self.sw_edges = []											# list of sw edges between fences
		self.sb_edges = []											# list of sb edges between fences and instructions combined
		self.fence_sb = []											# list of sb edges between fences
		self.fences = []											# list of all fences
		self.cycles = []											# list of all cycles between the fences

		f=0															# flag for finding execution trace
		for line in p.split('\n'):
			if f==2:
				if "HASH" in line:									# indicates end of one execution trace
					f=0
					self.traces.append(trace_list)
				else:
					trace_list.append(line.split())					# collect data from the execution trace and convert to list structure
 
			if f==1:
				f=2

			# check assertion violation case
			if 'Rf' in line:
				trace_list = []
				f=1

		self.loc = []												# list of locations of the required fence insertions

		for trace in self.traces:									# run for each trace
			order=self.fence(trace)

			# print("\norder=",order)
			# print("\nfence sb=",self.fence_sb)
			# print("\nsb=",self.sb_edges)
			
			sw(order,self.fence_sb,self.sb_edges)

			
	def fence(self,trace):
		# find out the number of threads in the program
		threads = 0
		for a in trace:
			threads = max(threads,int(a[1]))

		exec = []

		for j in range(1,threads+1):
			fences=0
			instr_no = 0
			for i in range(len(trace)):
				if int(trace[i][1])==j:
					fences+=1
					exec.append('F'+str(j)+str(fences))
					self.fences.append('F'+str(j)+str(fences))
					event = {'no': trace[i][0],						# trace[i][0] is the event number
							'thread': j,
							'no_in_thread': instr_no,
							'model': trace[i][4]
					}
					instr_no += 1
					if trace[i][3]=='read':
						event["type"] = "read"
						event['rf'] = trace[i][7]					# trace[i][7] gives Read-from (Rf)
					elif trace[i][3]=='write':
						event["type"] = "write"
					exec.append(event)
			fences+=1
			exec.append('F'+str(j)+str(fences))
			self.fences.append('F'+str(j)+str(fences))
		
		self.sb(exec,threads)
		# self.sb(exec,threads)
					
		return exec

	def sb(self, trace, threads):

		self.fence_sb = []											# list to store sb's of an execution
		self.sb_edges = []

		for i in range(1,threads+1):
			sb = []													# list to store sb's of each thread
			for j in range(1,len(trace)):
				if 'thread' in trace[j] and trace[j]['thread']==i:
					t = (trace[j-1],trace[j+1])
					if t not in sb:
						self.fence_sb.append(t)
				
				t0 = trace[j-1]
				t1 = trace[j]
				if not ('no' not in t0 and 'no' not in t1):
					if 'no' in t0:
						t0 = t0['no']
					if 'no' in t1:
						t1 = t1['no']
					if (t0,t1) not in self.sb_edges:
						self.sb_edges.append((t0,t1))

		fence_sb_temp = self.fence_sb
		flag = 0
		while flag != 2:
			for i in self.fence_sb:
				f1 = i[0]
				f2 = i[1]

				for j in self.fence_sb:
					if j[0] == f2:
						f3 = j[1]
						self.fence_sb.append((f1,f3))
				
			if fence_sb_temp == self.fence_sb:
				flag += 1
				
			fence_sb_temp = self.fence_sb

		self.fence_sb = list(dict.fromkeys(self.fence_sb))
		self.fence_sb.sort(key = lambda x: x[0])


		flag = 0
		while flag!=100:
			# if flag == 2:
				# break
			for i in self.sb_edges:
				e1 = i[0]
				e2 = i[1]

				for j in self.sb_edges:
					e3 = j[0]
					if e2 == e3:
						self.sb_edges.append((e1,j[1]))
						self.sb_edges = list(set(self.sb_edges))
				
			# if temp == sb:
			flag += 1
			
			# temp = list(set(sb))

		self.sb_edges = sorted(self.sb_edges)
		# print(self.sb_edges)
			
