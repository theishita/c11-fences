# --------------------------------------------------------
# Creates a structured list
# --------------------------------------------------------

def create_list(trace_no,instr,file_vars,trace_locs,threads_separated):
	line = []

	line.append(instr[0])								# 0: serial number of the instruction
	line.append(instr[1])								# 1: thread number

	if instr[2] == "init":
		line.append(instr[2])							# 2: instruction type (if init)
	else:
		line.append(instr[3])							# 2: instruction type (for the rest operation types)

	line.append(instr[4])								# 3: memory order
	line.append(instr[5])								# 4: memory address

	value = instr[6]
	# CDS Checker sets non-zero values as 0x1, 0x2 instead of 1, 2
	pos = value.find('x')
	new_value = ''
	if not pos == -1:
		new_value = value[pos+1:len(value)]
	else:
		new_value = value
	new_value = int(new_value, 16)
	line.append(new_value)								# 5: value

	if instr[3] == "read" or instr[3] == "rmw":
		line.append(instr[7])							# 6: rf value
	else:
		line.append("NA")								# 6: rf value = NA in case of non-read operation

	try:
		required_location = trace_locs.index(instr[5])	# find the required memory address from the list of memory addresses
		line.append(file_vars[required_location])		# 7: variable name
	except:
		line.append("NA")								# 7: variable name = NA in case of non-atomic operation

														# 8: line number, which will be added later

	return line
