# --------------------------------------------------------
# Gets an output form the CDS Model Checker and translates
# it into a required format.
# --------------------------------------------------------

import os
import subprocess
import shlex
import time
import sys
from operator import itemgetter

from .create_list import create_list
from constants import file_info as fi
from .delete_file import delete_copied_file

class translate_cds:
	def __init__(self, filename, no_traces):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.no_buggy_execs = 0											# number of buggy executions for this run
		self.error_string = ""											# error in CDS Checker
		self.cds_time = 0

		copy = "cp " + filename + " " + fi.CDS_TEST_FOLDER_PATH
		make = "cd "+ fi.CDS_FOLDER_PATH + " && make"

		input_file = filename.split('/')
		input_file = input_file[-1]

		input_file = "test/"+input_file[:-2]+'o'
		cds_cmd = './run.sh '+input_file								# cmd to run cds checker
		if no_traces:
			cds_cmd += " -c "+str(no_traces)
		cds_cmd = shlex.split(cds_cmd)

		os.system(copy)													# copy input file to cds checker directory
		cds_start = time.time()
		os.system(make)													# make/compile into object file for CDS Checker
		try:
			p = subprocess.check_output(cds_cmd,
										cwd = fi.CDS_FOLDER_PATH,
										stderr=subprocess.PIPE)				# get std output from CDS Checker
			cds_end = time.time()
			p = p.decode('utf-8')											# convert to string
			
			delete_copied_file(filename)
			self.cds_time = cds_end-cds_start
			self.obtain_traces(p)
		except:
			self.error_string = "error"

		if self.error_string != "error":
			self.no_buggy_execs = int(self.no_buggy_execs)
			print("\n\nBuggy executions:\t",self.no_buggy_execs)

			if self.no_buggy_execs != 0:
				self.create_structure(filename)

	def get(self):
		return self.traces, self.cds_time, self.no_buggy_execs, self.error_string

	# to differentiate and obtain each trace from the std output in the terminal
	def obtain_traces(self,p):
		f=0                                                         	# flag for finding execution trace
		for line in p.split('\n'):
			if f==2:
				if "HASH" in line:                                  	# indicates end of one execution trace
					f=0
					self.traces_raw.append(trace_list)
				else:
					trace_list.append(line.split())                 	# collect data from execution trace and convert to list structure

			if f==1:
				f=2

            # the start of a trace/assertion violation case
			if 'Rf' in line:
				trace_list = []
				f=1

			# print number of buggy executions
			if "Number of buggy executions" in line:
				self.no_buggy_execs = line[28:len(line)]
			
			if "******* Model-checking complete: *******" in line:
				self.error_string = ""
		
	# to convert each trace into a structure
	def create_structure(self,filename):

		# map the variable names from the source code to the memory address used by the variable as shown in the traces
		for trace in self.traces_raw:
			execution = []
			for instr in trace:
				line = create_list(instr)
				execution.append(line)
			execution.sort(key = lambda x:x[1])							# sorts the list of instructions by the thread number
			# IDEA: using key function is faster since it is called exactly once for each input record

			self.traces.append(execution)

		# for i in self.traces[S_NO]:
		# 	print(i)