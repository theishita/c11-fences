def separate_by_thread(filename):
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

		if "static void" in l or "user_main" in l:
			in_thread = 1
			brackets += 1

	# circular shift the list by 1 to send last element to first position
	# this is done to order the threads according to their indices where pos 0 will have the main thread, pos 1 will have thread 1 etc
	threads_separated.insert(0, threads_separated.pop())

	# print(threads_separated)
	for i in threads_separated:
		print(i)

	return threads_separated
