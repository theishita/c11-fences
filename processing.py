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
            order=self.fence(trace)
            print(order)

            hb_graph = hb(trace)
            mat,vertex_map,instr,size = hb_graph.get()
            
            get_mo = mo(mat,vertex_map,instr,size)
            mo_edges = get_mo.get()

            get_to = to(order,mo_edges,self.sb_edges)
            to_edges = get_to.get()

            print("sb fence=",self.sb_edges)
            print("list of all fences=",self.fences)

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

        for j in range(1,threads+1):
            fences=0
            instr_no = 0
            for i in range(len(trace)):
                if int(trace[i][1])==j:
                    fences+=1
                    exec.append('F'+str(j)+str(fences))
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
            self.fences.append('F'+str(j)+str(fences))
        
        self.sb(exec,threads)
                    
        return exec

    def sb(self, trace, threads):

        self.sb_edges = []                                                # list to store sb's of an execution

        for i in range(1,threads+1):
            sb = []                                                       # list to store sb's of each thread
            for j in range(len(trace)):
                if 'thread' in trace[j] and trace[j]['thread']==i:
                    t = (trace[j-1],trace[j+1])
                    if t not in sb:
                        self.sb_edges.append(t)

        sb_temp = self.sb_edges
        flag = 0

        while flag != 2:
            for i in self.sb_edges:
                f1 = i[0]
                f2 = i[1]

                for j in self.sb_edges:
                    if j[0] == f2:
                        f3 = j[1]
                        self.sb_edges.append((f1,f3))
                
            if sb_temp == self.sb_edges:
                flag += 1
                
                sb_temp = self.sb_edges
        
        self.sb_edges = list(dict.fromkeys(self.sb_edges))
        self.sb_edges.sort(key = lambda x: x[0])

        # self.events_order.append(exec)
        # self.sw(exec)

    # def sw(self,trace):
    #     sw = []                                                         # list of sw edges in this execution
    #     for i in range(len(trace)):
    #         if 'rf' in trace[i]:                                        # if the event is a write
    #             rf = trace[i]['rf']
    #             f1 = trace[i+1]

    #             for j in range(len(trace)):
    #                 if 'no' in trace[j] and trace[j]['no']==rf:
    #                     f2 = trace[j-1]
    #                     sw.append((f2,f1))
        
    #     self.sw_edges.append(sw)