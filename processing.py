

class Processing:
    def __init__(self,p):

        self.traces = []                                        # contains lists of all execution traces

        f=0                                                     # flag for finding execution trace
        for line in p.split('\n'):


            if f==2:
                if "HASH" in line:                              # indicates end of one execution trace
                    f=0
                    self.traces.append(trace)
                else:
                    trace.append(line.split())                  # collect data from the execution trace and convert to list structure
 
            if f==1:
                f=2

            # check assertion violation case
            if 'Rf' in line:
                trace = []
                f=1
        
        for a in self.traces:
            print("trace n")
            for b in a:
                print(b)

        
