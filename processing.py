# --------------------------------------------------------
# Processes the std:out data from the command line output
# and curates lists of:
#   1. Order of events including pseudo fences
#   2. Happens-before relationships (HB)
#   3. Modification Order relationships (MO)
#   4. Sequenced-before relationships (SB)
#   5. Total order relationships (TO)
#
# Then proceeds on to find out cycles from the TO graph
# and finally insert the required fences in the input
# program.
# --------------------------------------------------------

from stitch_z3 import Z3
from hb import hb
from graph import Graph
from mo import mo
from to import to
from cycle import Cycles
from insert import insert

class Processing:
    def __init__(self,p,filename):

        print(filename)

        self.traces = []                                            # lists of all execution traces
        self.events_order = []                                      # order of events including fences
        self.sw_edges = []                                          # list of sw edges between fences
        self.sb_edges = []                                          # list of sb edges between fences
        self.fences = []                                            # list of all fences
        self.fence_sb = []                                          # list of fences separated by sb's
        self.cycles = []                                            # list of all cycles between the fences

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

        self.loc = []                                               # list of locations of the required fence insertions

        for trace in self.traces:                                   # run for each trace

            hb_graph = hb(trace)
            mat,vertex_map,instr,size = hb_graph.get()
            
            get_mo = mo(mat,vertex_map,instr,size)
            mo_edges = get_mo.get()

            order=self.fence(trace)
            # print(order)

            get_to = to(order,mo_edges,self.sb_edges)
            to_edges = get_to.get()

            # print("sb fence=",self.sb_edges)
            # print("list of all fences=",self.fences)
            # print("fences in sb=",self.fence_sb)

            get_cycle = Cycles(self.fences,self.sb_edges,to_edges)
            cycle = get_cycle.get()
            for fence in cycle:
                for i in range(len(order)):
                    if fence == order[i]:
                        self.loc.append(order[i-1])

        self.loc = [i for n, i in enumerate(self.loc) if i not in self.loc[:n]] 
        # print("req locs=",self.loc)
        insert(self.loc,filename)


    def fence(self,trace):

        # find out the number of threads in the program
        threads = 0
        for a in trace:
            threads = max(threads,int(a[1]))

        exec = []
        self.fence_sb = []

        for j in range(1,threads+1):
            fences=0
            instr_no = 0
            fence_thread = []
            for i in range(len(trace)):
                if int(trace[i][1])==j:
                    fences+=1
                    exec.append('F'+str(j)+str(fences))
                    fence_thread.append('F'+str(j)+str(fences))         # fence order in a thread
                    self.fences.append('F'+str(j)+str(fences))
                    event = {'no': trace[i][0],                         # trace[i][0] is the event number
                            'thread': j,
                            'no_in_thread': instr_no
                    }
                    instr_no += 1
                    if trace[i][3]=='read':
                        event["type"] = "read"
                        event['rf'] = trace[i][7]                       # trace[i][7] gives Read-from (Rf)
                    elif trace[i][3]=='write':
                        event["type"] = "write"
                    exec.append(event)
            fences+=1
            exec.append('F'+str(j)+str(fences))
            fence_thread.append('F'+str(j)+str(fences))
            self.fences.append('F'+str(j)+str(fences))
            self.fence_sb.append(fence_thread)
        
        self.sb(exec,threads)
                    
        return exec

    def sb(self, trace, threads):

        self.sb_edges = []                                                # list to store sb's of an execution

        for i in self.fence_sb:
            for j in range(len(i)):
                for k in range(j+1,len(i)):
                    self.sb_edges.append((i[j],i[k]))      

        self.sb_edges = list(dict.fromkeys(self.sb_edges))
        self.sb_edges.sort(key = lambda x: x[0])