# --------------------------------------------------------
# Cleans up the generated "_fence" file in case -t 
# optimization flag is used and the solution cannot be
# found after a few iterations.
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

	# remove made file
	for ext in fi.CDS_MADE_FILE_EXTENSIONS:
		made_file_path = fi.CDS_TEST_FOLDER_PATH+"/."+filename[:-1*len(fi.FILE_EXTENSION)] + ext
		print(made_file_path)
		if os.path.exists(made_file_path):
			os.remove(made_file_path)
