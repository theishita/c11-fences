import ast

class z3translate:

    def __init__(self,cycles,var):

        self.constants = []                                         # list of all constant declarations
        conjunctions = []                                           # list of all cycles translated into conjunctions
        self.translation = ""
        self.loc_info = var

        for i in var.keys():
            name = var[i]
            self.constants.append(name)
        
        cycles_translated_to_var = []
        for i in range(len(cycles)):
            cycles_translated_to_var.append([var[x] for x in cycles[i]])
        
        for cycle in cycles_translated_to_var:
            conjunctions.append(self.conjunct(cycle))

        self.translation = self.disjunct(conjunctions)

        # print(self.constants, conjunctions, self.translation)

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
