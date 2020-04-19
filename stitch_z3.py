# --------------------------------------------------------
# Creates constants and assertions out of the given
# variables and conditions for the Z3 sat solver in the
# SMT programming language for Z3.
# --------------------------------------------------------

class convert_z3:

	def __init__(self,consts,disjunctions):
		file = open("c11","w")
		contents = ""

		conjunction = ""

		# creating the constants
		for var in consts:
			contents+= self.constant(var,'(_ BitVec 1)')

		if len(disjunctions)>1:
			conjunction = self.conjunct(disjunctions)
		else:
			conjunction = disjunctions[0]

		contents += self.fact('=',conjunction,'#b1')
		contents += self.minimize("bvadd",consts)
		contents+="(check-sat)\n(get-model)"
		file.write(contents)

	# to declare and define a constant
	def constant(self,name,dtype):
		cnst = "(declare-const "+name+" "+dtype+")\n"
		return cnst

	# to add assertions/facts
	def fact(self,operator,left,right):
		assertion = "(assert ("+operator+" "+left+" "+str(right)+"))\n"
		return assertion

    # to return a conjunction of given variables
	def conjunct(self,vars):
		cnf = "(bvand"
		for i in vars:
			cnf += " "+str(i)
		cnf += ")"
		return cnf

	# to create a minimize function
	def minimize(self,operator,variables):
		mini = "(minimize ("+operator
		for var in variables:
			mini += " "+var
		mini += "))\n"
		return mini

	# to declare and define a function
	def function(self,name,inputs,output):
		fn = "(declare-fun "+name+" ("
		for val in inputs:
			fn+=val+" "
		fn+=") "+output+")\n"
		return fn
