
# to declare and define a constant
def constant(name,dtype):
	cnst = "(declare-const "+name+" "+dtype+")\n"
	return cnst

# to add assertions/facts
def fact(operator,left,right):
	assertion = "(assert ("+operator+" "+left+" "+str(right)+"))\n"
	return assertion

# to create a generic function with an operator (parameterized by brackets)
def operation(operator,vars):
	if len(vars) == 1:
		return vars[0]

	formula = "("+operator
	for i in vars:
		formula += " "+str(i)
	formula += ")"
	return formula

# to return a conjunction of given variables
def conjunct(vars):
	cnf = operation("bvand",vars)
	return cnf

# to return a disjunction of given variables
def disjunct(vars):
	dnf = operation("bvor",vars)
	return dnf

# to create a minimize function
def minimize(formula):
	mini = "(minimize " + formula + ")\n"
	return mini

# to create a maximize function
def maximize(formula):
	maxi = "(maximize " + formula + ")\n"
	return maxi

# to declare and define a function
def function(name,inputs,fn_type,definition):
	fn = "(define-fun "+name+" ("
	for val in inputs:
		fn+="("+val+") "
	fn += ") "+fn_type+" "+definition+")\n"
	return fn
