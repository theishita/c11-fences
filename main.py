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

start = time.time()													# to calculate total tool time

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True)
# parser.add_argument("--help","-h", type=)

args = parser.parse_args()
filename = args.file												# gets the input file name

cds = translate_cds(filename)										# translates CDS Checker output & returns a structure containing the traces
traces,cds_time = cds.get()

get_p = Processing(traces)
loc_info = get_p.get()												# runs and returns locations

z3_cmd = 'z3 c11'
z3_run = shlex.split(z3_cmd)										# run z3 file

z3_start = time.time()
z3 = subprocess.check_output(z3_run,
							stderr=subprocess.PIPE)					# get std output after running z3
z3_end = time.time()
z3 = z3.decode('utf-8')

req_locs = z3run(z3)												# decipher output from z3 & get required locations
insert(req_locs,filename)											# insert fences into the source file at the requiren locations

end = time.time()

print("\nNumber of fences added:\t\t",len(req_locs))
print("Fences added after lines:\t",req_locs)

tool_time = end-start
z3_time = z3_end-z3_start
print("CDS Checker time:\t\t",cds_time)
print("Z3 time:\t\t\t",z3_time)
print("Total time:\t\t\t",tool_time)
print("Tool only time:\t\t\t",tool_time-z3_time-cds_time)