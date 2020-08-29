# --------------------------------------------------------
# Inputs a c++ file and outputs fences and fence positions
# with respect to C++11 version compiler.
#
# main.py runs the entire program.
# Usage:
#   python3 main.py -f [input file]
# --------------------------------------------------------

import argparse
import os
import subprocess
import shlex
import time

from processing import Processing
from z3run import z3run
from insert import insert
from translators.cds_checker.cds_checker import translate_cds

z3_time = 0

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True)
# parser.add_argument("--help","-h", type=)

args = parser.parse_args()
filename = args.file												# gets the input file name

start = time.time()													# to calculate total tool time
cds = translate_cds(filename)										# translates CDS Checker output & returns a structure containing the traces
traces,cds_time,conver_time,buggy_execs = cds.get()
# print("traces=",traces)

if buggy_execs:
	get_p = Processing(traces)
	loc_info, error_string, total_time = get_p.get()				# runs and returns locations

	if not error_string:
		z3_cmd = 'z3 compute_fences'
		z3_run = shlex.split(z3_cmd)								# run z3 file

		z3_start = time.time()
		z3 = subprocess.check_output(z3_run,
									stderr=subprocess.PIPE)			# get std output after running z3
		z3_end = time.time()
		z3 = z3.decode('utf-8')
		
		req_locs = z3run(z3)										# decipher output from z3 & get required locations
		insert(req_locs,filename)									# insert fences into the source file at the requiren locations

		print("Fences added:\t\t",len(req_locs))
		# print("Fences added after lines:\t",req_locs)
	
	else:
		print(error_string)

end = time.time()
tool_time = end-start

print("CDS Checker time:\t",round(cds_time,2))
if buggy_execs and not error_string:
	z3_time = z3_end-z3_start
	print("Z3 time:\t\t",round(z3_time,2))
print("Total time:\t\t",round(tool_time,2))
print("Tool only time:\t\t",round(tool_time-z3_time-cds_time,2))

print("\n\n\n======= CALCULATIONS TIMES =======")
print("\t\t\tTotal\t\tAvg per trace")
print("Conversion time :\t",round(conver_time, 2),"\t\t",round(conver_time/buggy_execs, 2))
print("HB :\t\t\t",round(total_time[0], 2),"\t\t",round(total_time[0]/buggy_execs, 2))
print("MO :\t\t\t",round(total_time[1], 2),"\t\t",round(total_time[1]/buggy_execs, 2))
print("Fences :\t\t",round(total_time[2], 2),"\t\t",round(total_time[2]/buggy_execs, 2))
print("TO :\t\t\t",round(total_time[3], 2),"\t\t",round(total_time[3]/buggy_execs, 2))
print("Cycles :\t\t",round(total_time[4], 2),"\t\t",round(total_time[4]/buggy_execs, 2))