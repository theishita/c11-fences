# --------------------------------------------------------
# Processes the std:out data from the command line output
# and creates lists of Synchronized-Waits (sw) and 
# Sequenced-Befores (sb).
# --------------------------------------------------------

from stitch_z3 import Z3
from hb import hb

class Processing:
    def __init__(self,p):

        self.traces = []                                            # lists of all execution traces
        self.events_order = []                                      # order of events including fences
        self.sw_edges = []                                          # list of sw edges between fences
        self.sb_edges = []                                          # list of sb edges between fences

        f=0                                                         # flag for finding execution trace
        for line in p.split('\n'):
            if f==2:
                if "HASH" in line:                                  # indicates end of one execution trace
                    f=0
                    self.traces.append(trace_list)
                else:
                    trace_list.append(line.split())                 # collect data from the execution trace and convert to list structure
 
            if f==1:
                f=2

            # check assertion violation case
            if 'Rf' in line:
                trace_list = []
                f=1

        for trace in self.traces:                                   # run for each trace
            self.fence(trace)
            hb(trace)

        # print(self.events_order)
        # print("sw:",self.sw_edges)
        # print("sb:",self.sb_edges)

        # Z3(self.sb_edges[0],self.sw_edges[0])
        
    def fence(self,trace):

        # find out the number of threads in the program
        threads = 0
        for a in trace:
            threads = max(threads,int(a[1]))

            
        # print("trace n")

        exec = []

        for j in range(2,threads+1):
            fences=0
            for i in range(len(trace)):
                if int(trace[i][1])==j:
                    fences+=1
                    exec.append('F'+str(j)+str(fences))
                    event = {'no': trace[i][0],                         # trace[i][0] is the event number
                            'thread': j
                    }
                    if trace[i][3]=='read':
                        event["type"] = "read"
                        event['rf'] = trace[i][7]                       # trace[i][7] gives Read-from (Rf)
                    elif trace[i][3]=='write':
                        event["type"] = "write"
                    exec.append(event)
            fences+=1
            exec.append('F'+str(j)+str(fences))
        
        self.events_order.append(exec)
        self.sw(exec)
        self.sb(exec,threads)

    def sw(self,trace):
        sw = []                                                         # list of sw edges in this execution
        for i in range(len(trace)):
            if 'rf' in trace[i]:                                        # if the event is a write
                rf = trace[i]['rf']
                f1 = trace[i+1]

                for j in range(len(trace)):
                    if 'no' in trace[j] and trace[j]['no']==rf:
                        f2 = trace[j-1]
                        sw.append((f2,f1))
        
        self.sw_edges.append(sw)

    def sb(self, trace, threads):

        edges = []                                                          # list to store sb's of an execution

        for i in range(2,threads+1):
            sb = []                                                         # list to store sb's of each thread
            for j in range(len(trace)):
                if 'thread' in trace[j] and trace[j]['thread']==i:
                    t = (trace[j-1],trace[j+1])
                    if t not in sb:
                        sb.append(t)
  
            edges.append(sb)

        self.sb_edges.append(edges)