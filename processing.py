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

from hb import hb
from graph import Graph
from mo import mo
from to import to
from cycle import Cycles
from insert import insert
from instructions_z3 import translate_z3
from stitch_z3 import convert_z3

import sys

class Processing:
    def __init__(self,p):

        self.traces = []                                            # lists of all execution traces
        self.events_order = []                                      # order of events including fences
        self.sw_edges = []                                          # list of sw edges between fences
        self.sb_edges = []                                          # list of sb edges between fences
        self.fences = []                                            # list of all fences
        self.fence_sb = []                                          # list of fences separated by sb's
        self.cycles = []                                            # list of all cycles between the fences
        self.constants = []                                         # list of all z3 constants
        self.disjunctions = []                                      # list of disjunctions for the z3 function
        self.loc_info = []                                          # information regaring the required fence locations

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

            if "Number of buggy executions" in line:
                print('\n\n',line)

        trace_no = 0
        for trace in self.traces:                                   # run for each trace

            trace_no += 1
            loc = []                                               # list of locations of the required fence insertions

            hb_graph = hb(trace)
            mat,vertex_map,instr,size = hb_graph.get()

            get_mo = mo(mat,vertex_map,instr,size)
            mo_edges = get_mo.get()

            order=self.fence(trace)

            get_to = to(order,mo_edges,self.sb_edges)
            to_edges = get_to.get()

            # print("sb fence=",self.sb_edges)
            # print("list of all fences=",self.fences)
            # print("fences in sb=",self.fence_sb)

            get_cycle = Cycles(self.fences,self.sb_edges,to_edges)
            cycles = get_cycle.get()
            unique_fences = list(sorted(set(x for l in cycles for x in l)))

            if len(unique_fences)>0:
                for fence in unique_fences:
                    for i in range(len(order)):
                        if fence == order[i]:
                            o = order[i-1]
                            var_name = 't'+str(o['thread'])+'n'+str(o['no_in_thread'])
                            temp = {'thread': o['thread'],
                                    'no_in_thread': o['no_in_thread'],
                                    'var_name': var_name,
                                    'fence': order[i]}
                            if temp not in loc:
                                loc.append(temp)
                                temp2 = {'thread': temp['thread'],
                                        'no_in_thread': temp['no_in_thread'],
                                        'var_name': temp['var_name']}
                                if temp2 not in self.loc_info:
                                    self.loc_info.append(temp2)

                get_translation = translate_z3(cycles,loc)
                constants, translation = get_translation.get()
                for i in constants:
                    self.constants.append(i)
                self.disjunctions.append(translation)
            else:
                print("No TO cycles can be formed for trace",trace_no,"\nHence this behaviour cannot be stopped using SC fences")
                sys.exit()

        self.constants = list(sorted(set(self.constants)))

        convert_z3(self.constants,self.disjunctions)

        # loc = [i for n, i in enumerate(loc) if i not in loc[:n]]
        # print("req locs=",self.loc_info)


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

    def get(self):
        return self.loc_info
