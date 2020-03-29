# --------------------------------------------------------
# Translates the output from Z3 and maps them to the
# required fence locations.
# --------------------------------------------------------

class run_z3:
	def __init__(self,loc_info,output):

		self.req_locs = []													# list of the final required locations

		output = output.split('\n')
		in_model = 0														# flag to tell whether the output is describing the model or not
		for i in range(len(output)):
			line = output[i]
			if line == ')':
				in_model -= 1												# model is finished now

			if in_model:
				if 'define-fun' in line:
					l_ind = self.get_var(line)
					var = line[14:l_ind].split()[0]							# starting index of variable name = 14
					val = output[i+1][6]									# variable's value (0 or 1)

					if val == '1':											# if value is one, the location is a required location
						self.req_locs.append(var[1:])

			if line == '(model ':
				in_model += 1												# model is starting now

	def get(self):
		return self.req_locs

	# obtains index location where the variable name ends
	def get_var(self,line):
		b = 0
		for l in range(len(line)):
			if b == 2:
				return l-1
			if line[l] == '(':
				b += 1
