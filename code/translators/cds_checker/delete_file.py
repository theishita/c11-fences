# --------------------------------------------------------
# Cleans up all generated files:
# 1. Generated file when -t optimization flag is used and
# 	the solution cannot be found after a few iterations
# 2. Copied and generated model checker files
# --------------------------------------------------------

import os
from constants import file_info as fi

def delete_generated_file(filename):
	if fi.OUTPUT_FILE_APPEND_STRING in filename:
		file_path = filename
		if os.path.exists(file_path):
			os.remove(file_path)

def delete_copied_file(filename):
	# remove copied file
	filename = filename.split("/")[-1]
	file_path_original = fi.CDS_TEST_FOLDER_PATH+"/"+filename
	if os.path.exists(file_path_original):
		os.remove(file_path_original)

	# remove made files
	for ext in fi.CDS_MADE_FILE_EXTENSIONS:
		made_file_path1 = fi.CDS_TEST_FOLDER_PATH+"/."+filename[:-1*len(fi.FILE_EXTENSION)] + ext
		made_file_path2 = fi.CDS_TEST_FOLDER_PATH+"/"+filename[:-1*len(fi.FILE_EXTENSION)] + ext
		if os.path.exists(made_file_path1):
			os.remove(made_file_path1)
		if os.path.exists(made_file_path2):
			os.remove(made_file_path2)
