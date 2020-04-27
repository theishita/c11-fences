# --------------------------------------------------------
# Creates a structured list
# --------------------------------------------------------

# TODO: hard code line numbers

from .find_line_no import find_line_no

def create_list(trace_no,instr,file_vars,trace_locs,filename):
	line = []

	line.append(instr[0])								# 0: serial number of the instruction
	line.append(instr[1])								# 1: thread number

	if instr[2] == "init":
		line.append(instr[2])							# 2: instruction type if init
	else:
		line.append(instr[3])							# 2: instruction type for the rest operation types

	line.append(instr[4])								# 3: memory order
	line.append(instr[5])								# 4: memory address

	value = instr[6]
	pos = value.find('x')								# CDS Checker sets non-zero values as 0x1, 0x2 instead of 1,2
	new_value = ''
	if not pos == -1:
		new_value = value[pos+1:len(value)]
	else:
		new_value = value
	line.append(new_value)								# 5: value

	if instr[3] == "read" or instr[3] == 'rmw':
		line.append(instr[7])							# 6: rf value
	else:
		line.append("NA")								# 6: rf value = NA in case of non-read operation

	try:
		required_location = trace_locs.index(instr[5])	# find the required memory address from the list of memory addresses
		line.append(file_vars[required_location])		# 7: variable name
	except:
		line.append("NA")								# 7: variable name = NA in case of non-atomic operation

	if instr[3] == "read" or instr[3] == "write" or instr[3] == "rmw":
		line.append(create_instruction(line[7],line[2],line[3],line[5]))	# 8: the instruction as present in the source code

		no = find_line_no(filename,line[1],line[8])
		if no == None:
			print("\nTrace",trace_no,", thread",int(line[1])-1,":\nLine number cannot be determined for instruction",line[0],":",line[8])
			print("Please mention it", end=": ")
			no = input()
		line.append(str(no))					# 9: line number of the instruction in the source code

	else:
		line.append("NA")								# 8: instruction = NA in case of non-read/write/rmw operation
		line.append("NA")								# 9: line number = NA in case of non-read/write/rmw operation

	return line

# given a set of values, creates the actual instruction as given in the source code
def create_instruction(var,instr_type,mem_order,value):
	instruction = var+'.'							# each atomic instruction begins with the name of the variable

	if instr_type == "read":
		instruction += "load("
	elif instr_type == "write":
		instruction += "store("+value+", "			# add the value of the store operation to the instruction
	# elif instr_type == "rmw":


	# instruction += "memory_order_"+mem_order+")"

	return instruction
