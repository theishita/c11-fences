# --------------------------------------------------------
# Creates constants and assertions out of the given
# variables and conditions for the Z3 sat solver in the
# SMT programming language for Z3.
# --------------------------------------------------------
from constants import *

def z3convert(consts,disjunctions):
	file = open(Z3_FILE,"w")
	contents = ""

	# creating the constants
	for var in consts:
		contents+= constant(var,'(_ BitVec 1)')
	
	consts_len = len(consts)
	bin_len = len(bin(consts_len)) - 1
	fn_type = "(_ BitVec "+str(bin_len)+")"
	fn_def = "((_ zero_extend "+str(bin_len - 1)+") x)"

	consts_fn = []
	for var in consts:
		consts_fn.append("(ext "+var+")")

	# contents += maximize("bvand",disjunctions)
	contents += fact("=",conjunct(disjunctions),"#b1")
	contents += function("ext",["x (_ BitVec 1)"],fn_type,fn_def)
	contents += minimize("bvadd",consts_fn)
	contents+="(check-sat)\n(get-model)"
	file.write(contents)

# to declare and define a constant
def constant(name,dtype):
	cnst = "(declare-const "+name+" "+dtype+")\n"
	return cnst

# to add assertions/facts
def fact(operator,left,right):
	assertion = "(assert ("+operator+" "+left+" "+str(right)+"))\n"
	return assertion

# to return a conjunction of given variables
def conjunct(vars):
	cnf = "(bvand"
	for i in vars:
		cnf += " "+str(i)
	cnf += ")"
	return cnf

# to create a minimize function
def minimize(operator,variables):
	mini = "(minimize ("+operator
	for var in variables:
		mini += " "+var
	mini += "))\n"
	return mini

# to create a maximize function
# unused
def maximize(operator,variables):
	maxi = "(maximize ("+operator
	for var in variables:
		maxi += " "+var
	maxi += "))\n"
	return maxi

# to declare and define a function
def function(name,inputs,fn_type,definition):
	fn = "(define-fun "+name+" ("
	for val in inputs:
		fn+="("+val+") "
	fn += ") "+fn_type+" "+definition+")\n"
	return fn
