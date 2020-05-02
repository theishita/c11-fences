import os
import subprocess
import shlex

cds = "python3 main.py -f test-programs/peterson.cc"
cds = shlex.split(cds)

p = subprocess.check_output(cds,
							cwd=".",
							stderr=subprocess.PIPE)
p = p.decode('utf-8')

t = open('test-programs/peterson_test.txt','w')
for l in p:
	t.writelines(l)
