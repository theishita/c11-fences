def find_line_no(threads_separated,instr,prev_instr):

	instr_type = instr[2]

	if instr_type == "rmw":
		line_no = int(prev_instr[8])+1
		return line_no

	src_code = create_instruction(instr)

	c = 0																		# counter to denote number of occurences
	line_no = "NA"

	if instr_type == "read" or instr_type == "write":
		thread_no = int(instr[1])-1													# thread number of the instruction
		for i in threads_separated[thread_no]:
			if src_code in i[1]:
				c += 1
				line_no = i[0]

		if c != 1:
			line_no = int(prev_instr[8])+1

	return line_no

def create_instruction(instr):
	instr_type = instr[2]
	var = instr[7]
	value = instr[5]
	mem_order = instr[3]

	src_code = var+'.'															# each atomic instruction begins with the name of the variable

	if instr_type == "read":
		src_code += "load("
	elif instr_type == "write":
		src_code += "store("+value+", "											# add the value of the store operation to the instruction

	src_code += "memory_order_"+mem_order+")"

	return src_code
