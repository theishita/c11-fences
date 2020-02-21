# --------------------------------------------------------
# Inputs a c++ file and outputs fences and fence positions
# with respect to C++11 version compiler.
# 
# Runs the entire program.
# Usage:
#   python3 main.py -f [input file]
# --------------------------------------------------------

#TODO: check first if the std:output is empty or not. Only then proceed to processing

import argparse
import os
import subprocess
import shlex

from processing import Processing

parser = argparse.ArgumentParser()                                               
parser.add_argument("--file", "-f", type=str, required=True)
# parser.add_argument("--help","-h", type=)

args = parser.parse_args()

copy = 'cp '+args.file+' $HOME/code/model-checker/test'
make = 'cd $HOME/code/model-checker && make'

os.system(copy)														# copy input file to cds checker directory
os.system(make)														# make/compile into object file

input_file = args.file.split('/')
input_file = input_file[-1]

input_file = "test/"+input_file[:-2]+'o'
cds = './run.sh '+input_file										# run cds checker
cds = shlex.split(cds)

p = subprocess.check_output(cds, 
							cwd="/home/ij/code/model-checker",
							stderr=subprocess.PIPE)					# get std output
p = p.decode('utf-8')												# convert to string

filename = args.file
Processing(p,filename)