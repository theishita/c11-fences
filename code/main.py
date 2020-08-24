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
import sys

from processing import Processing
from z3run import z3run
from insert import insert
from translators.cds_checker.cds_checker import translate_cds

start = time.time()													# to calculate total tool time

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True, 
					help="Enter file path and name")
parser.add_argument("--traces", "-t", type=int, required=False, dest="no_traces",
					help="For a faster but less optimized output, enter an int value with this flag to check only this number of traces at once")
parser.add_argument("--max-iter", "-m", type=int, required=False, dest="max_iter",
					help="After entering traces, you can enter a maximum number of iterations as well to avoid going in an infinite loop for programs. You can only enter this after entering the number of traces")

args = parser.parse_args()

if args.max_iter is not None and args.no_traces is None:
	print("Please specify number of traces to be checked as well using flag -t")
	sys.exit(0)
elif args.no_traces == 0 or args.max_iter == 0:
	print("Values cannot be 0")
	sys.exit(0)

file = args.file												# gets the input file name
no_traces = args.no_traces										# gets the input number of traces to be checked
max_iter = args.max_iter										# gets the input maximum number of iterations

cds_total = 0
z3_total = 0
fences_added = 0
total_iter = 0

def fn_main(filename):
	global cds_total
	global z3_total
	global fences_added
	global total_iter

	if max_iter and total_iter == max_iter:
		return

	total_iter += 1
	print("\n\n=============== ITERATION",total_iter,"===============")

	cds = translate_cds(filename)								# translates CDS Checker output & returns a structure containing the traces
	traces,cds_time,no_buggy_execs = cds.get()

	if no_buggy_execs:
		no = no_traces if no_traces and no_traces < len(traces) else len(traces)
		get_p = Processing(traces, no)
		fences_present, error_string = get_p.get()				# creates the required z3 file for our usage

		if error_string:
			print(error_string)

		else:
			z3_cmd = 'z3 compute_fences'
			z3_run = shlex.split(z3_cmd)								# run z3 file

			z3_start = time.time()
			z3 = subprocess.check_output(z3_run,
										stderr=subprocess.PIPE)			# get std output after running z3
			z3_end = time.time()
			z3 = z3.decode('utf-8')

			req_locs = z3run(z3)										# decipher output from z3 & get required locations
			new_filename = insert(req_locs,filename,fences_present)		# insert fences into the source file at the requiren locations

			fences_added += len(req_locs)-len(fences_present)

			z3_time = z3_end-z3_start

	print("CDS Checker time:\t",cds_time)
	cds_total += cds_time
	if no_buggy_execs and not error_string:
		print("Z3 time:\t\t",z3_time)
		z3_total += z3_time
		print("Fences added:\t\t",len(req_locs)-len(fences_present))

	if no_traces and no_buggy_execs and not error_string:
		fn_main(new_filename)

	return

start = time.time()
fn_main(file)
end = time.time()
print("\n\n\n===========================================")
print("Total fences added:\t",fences_added)
print("Total iterations:\t",total_iter)
print("Total time taken:\t",end-start)
print("Tool only time taken:\t",end-start-cds_total-z3_total)
print("Avg time per iteration:\t",(end-start)/total_iter)