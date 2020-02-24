class sw:
	def __init__(self,order,fence_sb,sb):
		self.order = order
		self.fence_sb = fence_sb
		self.sb_edges = sb

		self.write_models = ["release","seq_cst"]
		self.read_models = ["acquire","seq_cst"]

		self.sw_edges = []
		self.fence_models = []

		self.rule1()
		self.rule2()
		self.rule3()
		self.rule4()

		print("sw=",self.sw_edges)
		print("fences=",self.fence_models)
	
	def rule1(self):
		for i in self.order:
			if 'rf' in i:
				if i['model'] in self.read_models:
					r = i['no']
					w = i['rf']

					for j in self.order:
						if 'no' in j:
							if j['no'] == w and j['model'] in self.write_models:
								self.sw_edges.append((w,r))

	def rule2(self):
		for i in range(len(self.order)):
			rinstr = self.order[i]
			if 'rf' in rinstr:
				w = rinstr['rf']

				f2 = {	'name': self.order[i+1],
						'model': '>=acquire'}

				for j in range(len(self.order)):
					winstr = self.order[j]
					if 'no' in winstr:
						if winstr['no'] == w:
							f1 = {	'name': self.order[j-1],
									'model': '>=release'}
							
							self.sw_edges.append((f1['name'],f2['name']))
							self.fence_models.append(f1)
							self.fence_models.append(f2)

	def rule3(self):
		for i in range(len(self.order)):
			rinstr = self.order[i]
			if 'rf' in rinstr:
				if rinstr['model'] in self.read_models:
					r = rinstr['no']
					w = rinstr['rf']

					for j in range(len(self.order)):
						winstr = self.order[j]
						if 'no' in winstr:
							if winstr['no'] == w:
								f = {'name': self.order[j-1],
									'model': '>=release'}
								self.sw_edges.append((f['name'],r))
								self.fence_models.append(f)
	
	def rule4(self):
		for i in range(len(self.order)):
			rinstr = self.order[i]
			if 'no' in rinstr:
				if 'rf' in rinstr:
					w = rinstr['rf']
					
					for j in self.order:
						if 'no' in j:
							if j['no'] == w and j['model'] in self.write_models:
								f = {'name': self.order[i+1],
									'model': '>=acquire'}
								self.sw_edges.append((w,f['name']))
								self.fence_models.append(f)
	
	# def all_sw(self):
