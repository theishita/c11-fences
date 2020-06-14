import ast

class z3translate:

    def __init__(self,cycles,var):

        self.constants = []                                         # list of all constant declarations
        conjunctions = []                                           # list of all cycles translated into conjunctions
        self.translation = ""
        self.loc_info = var

        to_sets_store = open("store/to_sets_store", 'r')
        to_sets = to_sets_store.read()
        to_sets_store.close()

        to_sets = ast.literal_eval(to_sets)

        for i in var.keys():
            name = var[i]
            self.constants.append(name)

        for cycle in cycles:
            cycle_set = []
            for node_index in range(len(cycle)):
                node1 = cycle[node_index]
                node2 = cycle[0] if node_index == (len(cycle) - 1) else cycle[node_index + 1]

                fence_sets = to_sets[(node1,node2)] if (node1,node2) in to_sets.keys() else [[]]

                if [] in fence_sets:
                    continue
                
                cycle_set.append(self.translate_fence_set(fence_sets))
            
            conjunctions.append(self.conjunct(cycle_set))
            

        self.translation = self.disjunct(conjunctions)

        # print(self.const_declarations, self.conjunctions, self.translation)

    def get(self):
        return self.constants, self.translation

    # to return a conjunction of given variables
    def conjunct(self,vars):
        if len(vars) == 1:
            return vars[0]

        cnf = "(bvand"
        for i in vars:
            cnf += " "+str(i)
        cnf += ")"
        return cnf

    # to return a disjunction of given variables
    def disjunct(self,vars):
        if len(vars) == 1:
            return vars[0]

        dnf = "(bvor"
        for i in vars:
            dnf += " "+str(i)
        dnf += ")"
        return dnf

    def translate_fence_set(self,fence_sets):
        conjunctions = []
        for a_set in fence_sets:
            vars = []
            for fence in a_set:
                vars.append(self.loc_info[fence])
            
            conjunctions.append(self.conjunct(vars))
        
        return self.disjunct(conjunctions)
