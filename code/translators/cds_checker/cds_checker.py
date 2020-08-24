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

from .map_var import map_var
from .create_list import create_list
from .separate_by_thread import separate_by_thread
from .find_line_no import find_line_no

class translate_cds:
	def __init__(self,filename):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.no_buggy_execs = 0											# number of buggy executions for this run

		copy = 'cp '+filename+' ../model-checker/test'
		make = 'cd ../model-checker && make'

		os.system(copy)													# copy input file to cds checker directory
		os.system(make)													# make/compile into object file for CDS Checker

		input_file = filename.split('/')
		input_file = input_file[-1]

		input_file = "test/"+input_file[:-2]+'o'
		cds = './run.sh '+input_file									# cmd to run cds checker
		cds = shlex.split(cds)

		cds_start = time.time()
		p = subprocess.check_output(cds,
									cwd="../model-checker",
									stderr=subprocess.PIPE)				# get std output from CDS Checker
		cds_end = time.time()
		p = p.decode('utf-8')											# convert to string

		self.cds_time = cds_end-cds_start

		self.obtain_traces(p)
		self.no_buggy_execs = int(self.no_buggy_execs)
		print("Buggy executions:\t",self.no_buggy_execs)

		if self.no_buggy_execs != 0:
			self.create_structure(filename)

	def get(self):
		return self.traces,self.cds_time,self.no_buggy_execs

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

	# to convert each trace into a structure
	def create_structure(self,filename):

		# map the variable names from the source code to the memory address used by the variable as shown in the traces
		get_var = map_var(self.traces_raw[0],filename)
		file_vars,trace_locs = get_var.get()

		threads_separated = separate_by_thread(filename)

		trace_no = 1
		for trace in self.traces_raw:
			execution = []
			for instr in trace:
				line = create_list(trace_no,instr,file_vars,trace_locs,threads_separated)
				execution.append(line)
			execution.sort(key = lambda x:x[1])							# sorts the list of instructions by the thread number
			# IDEA: using key function is faster since it is called exactly once for each input record

			for i in range(1,len(execution)):
				line_no = find_line_no(threads_separated,execution[i],execution[i-1])
				execution[i].append(line_no)
				# print(execution[i])

			trace_no += 1

			self.traces.append(execution)

		# for i in self.traces[0]:
		# 	print(i)
