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

class translate_cds:
	def __init__(self,filename):

		self.traces_raw = []													# list of all traces raw
		self.traces = []														# list of processed traces

		copy = 'cp '+filename+' $HOME/code/model-checker/test'
		make = 'cd $HOME/code/model-checker && make'

		os.system(copy)														# copy input file to cds checker directory
		os.system(make)														# make/compile into object file for CDS Checker

		input_file = filename.split('/')
		input_file = input_file[-1]

		input_file = "test/"+input_file[:-2]+'o'
		cds = './run.sh '+input_file										# run cds checker
		cds = shlex.split(cds)

		cds_start = time.time()
		p = subprocess.check_output(cds,
									cwd="../model-checker",
									stderr=subprocess.PIPE)					# get std output from CDS Checker
		cds_end = time.time()
		p = p.decode('utf-8')												# convert to string

		self.cds_time = cds_end-cds_start

		self.obtain_traces(p)
		self.create_structure(filename)

	def get(self):
		return self.traces,self.cds_time

	# to differentiate and obtain each trace from the std output in the terminal
	def obtain_traces(self,p):
		f=0                                                         		# flag for finding execution trace
		for line in p.split('\n'):
			if f==2:
				if "HASH" in line:                                  		# indicates end of one execution trace
					f=0
					self.traces_raw.append(trace_list)
				else:
					trace_list.append(line.split())                 		# collect data from the execution trace and convert to list structure

			if f==1:
				f=2

            # the start of a trace/assertion violation case
			if 'Rf' in line:
				trace_list = []
				f=1

			# print number of buggy executions
			if "Number of buggy executions" in line:
				print('\n')
				print(line)
				if line == "Number of buggy executions: 0":
					sys.exit()												# in case of zero buggy execs, exit program

	# to convert each trace into a structure
	def create_structure(self,filename):
		# TODO: add line Number

		# map the variable names from the source code to the memory address used by the variable as shown in the traces
		get_var = map_var(self.traces_raw[0],filename)
		file_vars,trace_locs = get_var.get()

		for trace in self.traces_raw:
			execution = []
			for instr in trace:
				line = create_list(instr,file_vars,trace_locs,filename)
				execution.append(line)
			execution.sort(key = lambda x:x[1])								# sorts the list of instructions by the thread number
			# IDEA: using key function is faster since it is called exactly once for each input record

			self.traces.append(execution)

		for i in self.traces[0]:
			print(i)
