# --------------------------------------------------------
# Creates a file suitable for Z3, gets the output and maps 
# them to the required fence locations.
# --------------------------------------------------------
import shlex
import subprocess
import time

from z3convert import z3convert

def z3run(z3vars, disjunctions):

	z3convert(z3vars, disjunctions)										# create the z3 file based on the cycles

	z3_cmd = 'z3 compute_fences'
	z3_run = shlex.split(z3_cmd)										# run z3 file

	z3_start = time.time()
	output = subprocess.check_output(z3_run,
								stderr=subprocess.PIPE)					# get std output after running z3
	z3_end = time.time()
	output = output.decode('utf-8')

	req_locs = []														# list of the final required locations

	output = output.split('\n')
	in_model = 0														# flag to tell whether the output is describing the model or not
	for i in range(len(output)):
		line = output[i]
		if line == ')':
			in_model -= 1												# model is finished now

		if in_model:
			if 'define-fun' in line:
				l_ind = get_var(line)
				var = line[14:l_ind].split()[0]							# starting index of variable name = 14
				val = output[i+1][6]									# variable's value (0 or 1)

				if val == '1':											# if value is one, the location is a required location
					req_locs.append(var[1:])

		if line == '(model ':
			in_model += 1												# model is starting now

	return req_locs, (z3_end-z3_start)

# obtains index location where the variable name ends
def get_var(line):
	b = 0
	for l in range(len(line)):
		if b == 2:
			return l-1
		if line[l] == '(':
			b += 1
