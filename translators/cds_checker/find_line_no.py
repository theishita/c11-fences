def find_line_no(filename,thread,instr):
	k = 0
	with open(filename) as f:
		k += 1
		lines = f.readlines()

	in_thread = 0
	brackets = 0
	thread_contents = []
	threads_separated = []
	line_no = 0
	for l in lines:
		line_no += 1
		if in_thread:
			if '}' in l:
				brackets -= 1
			if '{' in l:
				brackets += 1
			if brackets == 0:
				in_thread = 0
				threads_separated.append(thread_contents)
				thread_contents = []
				continue
			thread_contents.append((line_no,l))

		if "static void" in l:
			in_thread = 1
			brackets += 1

	# print(threads_separated)
	# for i in threads_separated:
	# 	print(i)

	thread_no = int(thread)-2
	for i in threads_separated[thread_no]:
		if instr in i[1]:
			return i[0]
