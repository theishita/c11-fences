class map_var:
    def __init__(self,trace,filename):
        self.file_vars = []
        self.trace_locs = []

        self.get_filevars(filename)
        self.get_tracelocs(trace)

    def get(self):
        return self.file_vars,self.trace_locs

    def get_filevars(self,filename):
        with open(filename) as f:
            lines = f.readlines()

        flag = 0
        for line in lines:
            if flag == 1:
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

    def get_tracelocs(self,trace):
        for t in trace:
            if t[2] == "init":
                loc = t[5]
                self.trace_locs.append(loc)
