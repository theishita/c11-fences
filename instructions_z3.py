class translate_z3:

    def __init__(self,cycles,var):

        self.constants = []                                        # list of all constant declarations
        conjunctions = []                                              # list of all cycles translated into conjunctions
        self.translation = ""

        for i in var:
            name = i['var_name']
            self.constants.append(name)

        for i in cycles:
            cyc = []
            for j in i:
                for k in var:
                    if j == k['fence']:
                        cyc.append(k['var_name'])

            cnf = self.conjunct(cyc)
            conjunctions.append(cnf)

        if len(conjunctions) > 1:
            self.translation = self.disjunct(conjunctions)
        else:
            self.translation = conjunctions[0]

        # print(self.const_declarations, self.conjunctions, self.translation)

    def get(self):
        return self.constants, self.translation

    # to return a conjunction of given variables
    def conjunct(self,vars):
        cnf = "(bvand"
        for i in vars:
            cnf += " "+str(i)
        cnf += ")"
        return cnf

    # to return a disjunction of given variables
    def disjunct(self,vars):
        dnf = "(bvor"
        for i in vars:
            dnf += " "+str(i)
        dnf += ")"
        return dnf
