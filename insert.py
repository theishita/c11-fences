from operator import itemgetter

class insert:
	def __init__(self,loc,filename):
		self.loc = loc
		self.filename = filename

		self.insert_fences()

	def insert_fences(self):

		with open(self.filename) as f:
			lines = f.readlines()
			# lines = [line.strip() for line in f]

		self.filename = self.filename[:-3]
		filename_new = self.filename+'_fence.cc'
		temp = open(filename_new,'w')
		fence_instr = 'atomic_thread_fence(memory_order_seq_cst);\n'

		for i in self.loc:
			l = int(i)
			lines[l-1] += fence_instr

		for w in lines:
			temp.writelines(w)
