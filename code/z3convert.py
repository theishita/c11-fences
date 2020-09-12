# --------------------------------------------------------
# Creates constants and assertions out of the given
# variables and conditions and writes it into the file.
# --------------------------------------------------------
from constants import *
import z3_functions as z3

def z3convert(consts,disjunctions):
	file = open(Z3_FILE,'w')
	contents = ""

	# creating the constants
	for var in consts:
		contents+= z3.constant(var,'(_ BitVec 1)')
	
	consts_len = len(consts)
	bin_len = len(bin(consts_len)) - 1
	fn_type = "(_ BitVec "+str(bin_len)+")"
	fn_def = "((_ zero_extend "+str(bin_len - 1)+") x)"

	consts_fn = []
	for var in consts:
		consts_fn.append("(ext "+var+")")

	# contents += maximize("bvand",disjunctions)
	contents += z3.fact("=",z3.conjunct(disjunctions),"#b1")
	contents += z3.function("ext",["x (_ BitVec 1)"],fn_type,fn_def)
	contents += z3.minimize(z3.operation("bvadd",consts_fn))
	contents+="(check-sat)"
	file.write(contents)