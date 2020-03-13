# --------------------------------------------------------
# Gets an output form the CDS Model Checker and translates
# it into a required format.
# --------------------------------------------------------

import os
import subprocess
import shlex
import time
import sys

class translate_cds:
	def __init__(self,filename):

		self.traces = []													# list of all traces

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

		print("Time taken by CDS Checker:",cds_end-cds_start)

		self.create_structure(p)
	
	def get(self):
		return self.traces

	def create_structure(self,p):
		f=0                                                         		# flag for finding execution trace
		for line in p.split('\n'):
			if f==2:
				if "HASH" in line:                                  		# indicates end of one execution trace
					f=0
					self.traces.append(trace_list)
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
		
		for i in self.traces:
			print(i)

        # get_var = map_var(self.traces[0],filename)
        # file_vars,trace_locs = get_var.get()
