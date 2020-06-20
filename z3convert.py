# --------------------------------------------------------
# Creates constants and assertions out of the given
# variables and conditions for the Z3 sat solver in the
# SMT programming language for Z3.
# --------------------------------------------------------

class z3convert:

	def __init__(self,consts,disjunctions):
		file = open("c11","w")
		contents = ""

		# creating the constants
		for var in consts:
			contents+= self.constant(var,'(_ BitVec 1)')
		
		consts_len = len(consts)
		bin_len = len(bin(consts_len)) - 2
		fn_type = "(_ BitVec "+str(bin_len)+")"
		fn_def = "((_ zero_extend "+str(bin_len - 1)+") x)"

		consts_fn = []
		for var in consts:
			consts_fn.append("(ext "+var+")")

		contents += self.maximize("bvand",disjunctions)
		contents += self.function("ext",["x (_ BitVec 1)"],fn_type,fn_def)
		contents += self.minimize("bvadd",consts_fn)
		contents+="(check-sat)\n(get-model)"
		file.write(contents)

	# to declare and define a constant
	def constant(self,name,dtype):
		cnst = "(declare-const "+name+" "+dtype+")\n"
		return cnst

	# to add assertions/facts
	# unused
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

	# to create a maximize function
	def maximize(self,operator,variables):
		maxi = "(maximize ("+operator
		for var in variables:
			maxi += " "+var
		maxi += "))\n"
		return maxi

	# to declare and define a function
	def function(self,name,inputs,fn_type,definition):
		fn = "(define-fun "+name+" ("
		for val in inputs:
			fn+="("+val+") "
		fn += ") "+fn_type+" "+definition+")\n"
		return fn
