# --------------------------------------------------------
# Cleans up the generated "_fence" file in case -t 
# optimization flag is used and the solution cannot be
# found after a few iterations.
# --------------------------------------------------------

import os
from constants import OUTPUT_FILE_APPEND_STRING

def delete_generated_file(filename):
	if OUTPUT_FILE_APPEND_STRING in filename:
		file_path = filename
		if os.path.exists(file_path):
			os.remove(file_path)