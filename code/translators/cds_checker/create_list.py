# --------------------------------------------------------
# Creates a structured list
# --------------------------------------------------------

def create_list(instr):
	line = []

	line.append(int(instr[0]))							# 0: serial number of the instruction
	line.append(int(instr[1]))							# 1: thread number

	line.append(instr[3])								# 2: instruction type

	line.append(instr[4])								# 3: memory order
	line.append(instr[5])								# 4: memory address/location

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
		line.append(int(instr[7]))						# 6: rf value
	else:
		line.append("NA")								# 6: rf value = NA in case of non-read operation

	if instr[3] == "read" or instr[3] == "rmw":
		line.append(int(instr[8]))						# 7: line number
	elif  instr[3] == "write":
		line.append(int(instr[7]))						# 7: line number
	else:
		line.append("NA")								# 7: line number = NA in case of non-read/write/rmw operation

	return line