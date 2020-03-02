class run_z3:
	def __init__(self,loc_info,output):

		self.req_locs = []										# final required locations

		output = output.split('\n')
		in_model = 0
		for i in range(len(output)):
			line = output[i]
			if line == ')':
				in_model -= 1
			
			if in_model:
				if 'define-fun' in line:
					var = line[14:18]
					val = output[i+1][6]

					if val == '1':
						for loc in loc_info:
							if loc['var_name'] == var:
								self.req_locs.append(loc)

			if line == '(model ':
				in_model += 1
		
	def get(self):
		return self.req_locs
