# --------------------------------------------------------
# Translates the output from Z3 and maps them to the
# required fence locations.
# --------------------------------------------------------

class run_z3:
	def __init__(self,loc_info,output):

		self.req_locs = []										# list of the final required locations

		output = output.split('\n')
		in_model = 0											# flag to tell whether the output is describing the model or not
		for i in range(len(output)):
			line = output[i]
			if line == ')':
				in_model -= 1									# model is finished now
			
			if in_model:
				if 'define-fun' in line:
					var = line[14:18]							# variable name
					val = output[i+1][6]						# variable's value (0 or 1)

					if val == '1':								# if value is one, the location is a required location
						for loc in loc_info:
							if loc['var_name'] == var:
								self.req_locs.append(loc)

			if line == '(model ':
				in_model += 1									# model is starting now
		
	def get(self):
		return self.req_locs
