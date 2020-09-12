import ast
import z3_functions as z3

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
            conjunctions.append(z3.conjunct(cycle))

        self.translation = z3.disjunct(conjunctions)

        # print(self.constants, conjunctions, self.translation)

    def get(self):
        return self.constants, self.translation
