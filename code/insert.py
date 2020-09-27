from operator import itemgetter
from constants import file_info as fi

def insert(loc, filename, fences_present_locs):
	with open(filename) as f:
		lines = f.readlines()
	
	if fi.OUTPUT_FILE_APPEND_STRING in filename:
		filename_new = filename
	else:
		extension_length = -1*len(fi.FILE_EXTENSION)
		filename_new = filename[:extension_length]+fi.OUTPUT_FILE_APPEND_STRING

	output_file = open(filename_new,'w')
	fence_instr = fi.FENCE_INSTRUCTION+'\n'

	for i in loc:
		if i not in fences_present_locs:
			lines[i-1] += fence_instr

	for w in lines:
		output_file.writelines(w)

	return filename_new
