from operator import itemgetter

class insert:
	def __init__(self,loc,filename,fences_present):

		with open(filename) as f:
			lines = f.readlines()

		filename = filename[:-3]
		filename_new = filename+'_fence.cc'
		output_file = open(filename_new,'w')
		fence_instr = 'atomic_thread_fence(memory_order_seq_cst);\n'

		for i in loc:
			if i not in fences_present:
				l = int(i[1:])
				lines[l-1] += fence_instr

		for w in lines:
			output_file.writelines(w)
