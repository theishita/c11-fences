

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

        self.fence()
        
    def fence(self):

        # find out the number of threads in the program
        threads = 0
        for a in self.traces:
            for b in a:
                threads = max(threads,int(b[1]))
        
        print("Number of threads (including main thread) =",threads)

        self.events_order = []                                  # order of events including fences

        for a in self.traces:
            fences = 1
            print("trace n")

            exec = ['F1']

            for j in range(2,threads+1):
                for i in range(len(a)):
                    if int(a[i][1])==j:
                        exec.append('E'+a[i][0])
                        fences+=1
                        exec.append('F'+str(fences))
            
            self.events_order.append(exec)

        print(self.events_order)
