import os
import subprocess
import shlex
import argparse
import time
import signal

from os import listdir
from os.path import isfile, join


def run_extra_files():
    files = ["benchmarks/cds_examples/mot_eg_modified.cc"]
    tempfile = open("benchmarks/cds_examples/output", 'a+')
    for f in files:
        print(f)
        cmd = "python3 main.py -f " + f
        cmd = shlex.split(cmd)

        output = subprocess.check_output(cmd, stderr=subprocess.PIPE)
        output = output.decode('utf-8')
        tempfile.write("\n\n\n===========================\n" + str(f) +
                       "\n===========================\n" + output)

    tempfile.close()


def run_file(dir_name, filename, tempfile):
    f = dir_name + "/" + filename
    print(f)
    cmd = "python3 main.py -f " + f
    cmd = shlex.split(cmd)

    output = subprocess.check_output(cmd, stderr=subprocess.PIPE)
    output = output.decode('utf-8')
    tempfile.write("\n\n\n===========================\n" + str(filename) +
                   "\n===========================\n" + output)

    # no_buggy_execs = 0
    # fences = 0
    # for line in output.split('\n'):
    #     if "Buggy executions" in line:
    #         no_buggy_execs = line[19:len(line)]
    #     if "Total fences added" in line:
    #         fences = line[33:len(line)][:-5]

    # if int(no_buggy_execs) > 0 and int(fences) > 0:
    #     tempfile.write("\n\n\n" + str(filename) + "\nBuggies: " +
    #                    str(no_buggy_execs) + "\nFences added: " + str(fences))
    # # print(output.find("Total fences added: \t"))


# directories = ["cds_examples/", "genmc_examples/", "misc/", "rcmc_examples/", "tracer_examples/", "watts_examples/"]
directories = [
    "benchmarks/cds_examples", "benchmarks/genmc_examples", "benchmarks/misc",
    "benchmarks/rcmc_examples", "benchmarks/tracer_examples",
    "benchmarks/watts_examples"
]
# directories = ["benchmarks/misc"]

# run_extra_files()

for dir_name in directories:
    tempfile_path = dir_name + "/output_avg_per_instr"
    tempfile = open(tempfile_path, 'a+')
    files = [f for f in listdir(dir_name) if isfile(join(dir_name, f))]
    for filename in files:
        run_file(dir_name, filename, tempfile)
    tempfile.close()
