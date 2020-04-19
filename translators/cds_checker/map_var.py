# --------------------------------------------------------
# Maps the variables from the memory addresses used in CDS
# Checker to the variable names used in the source code.
#
# As a rule, each variable used in the program needs to be
# initialized in the main function before the init command
# can be interrupted by anything else in the trace. That
# is, each example trace needs to necessarily have all the
# init variables before starting any threads.
# --------------------------------------------------------

class map_var:
    def __init__(self,trace,filename):
        self.file_vars = []                                 # list of variable names in the source code file
        self.trace_locs = []                                # list of memory address names in CDS Checker traces

        self.get_filevars(filename)
        self.get_tracelocs(trace)

    def get(self):
        return self.file_vars,self.trace_locs

    # gets variable names that are explicitly initialized in the source code in that order
    def get_filevars(self,filename):
        with open(filename) as f:
            lines = f.readlines()

        flag = 0                                            # flag to check if we are in the main function yet
        for line in lines:
            if flag == 1:                                   # if in the main function, initialized variables can be detected and stored
                if "atomic_init" in line:
                    pos = line.find("atomic_init")
                    var = ''
                    pos += 13
                    while not line[pos] == ',':
                        var += line[pos]
                        pos += 1
                    self.file_vars.append(var)
            if "user_main" in line:
                flag = 1

    # gets memory locations that have been explicitly initialized in the source code in that order
    def get_tracelocs(self,trace):
        for t in trace:
            if t[2] == "init":
                loc = t[5]                                  # t[5] gives the memory address of the initialized var
                self.trace_locs.append(loc)
