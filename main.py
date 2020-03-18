# --------------------------------------------------------
# Inputs a c++ file and outputs fences and fence positions
# with respect to C++11 version compiler.
#
# Runs the entire program.
# Usage:
#   python3 main.py -f [input file]
# --------------------------------------------------------

import argparse
import os
import subprocess
import shlex
import time

from processing import Processing
from run_z3 import run_z3
from insert import insert
from translators.cds_checker.cds_checker import translate_cds

start = time.time()

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True)
# parser.add_argument("--help","-h", type=)

args = parser.parse_args()
filename = args.file												# gets the input file name
cds = translate_cds(filename)										# gets the translation from the CDS Checker output and returns a structure containing the traces
traces,cds_time = cds.get()

get_p = Processing(traces)
loc_info = get_p.get()												# runs and returns locations

# z3_cmd = 'z3 c11'
# z3_run = shlex.split(z3_cmd)										# run z3 file

# z3_start = time.time()
# z3 = subprocess.check_output(z3_run,
# 							stderr=subprocess.PIPE)					# get std output after running z3
# z3_end = time.time()
# z3 = z3.decode('utf-8')

# get_locs = run_z3(loc_info,z3)										# decipher output from z3
# req_locs = get_locs.get()											# get required locations

# insert(req_locs,filename)											# insert fences into the source file at the requiren locations

# end = time.time()
