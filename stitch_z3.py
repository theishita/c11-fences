# --------------------------------------------------------
# Creates functions and realtions for the Z3 sat solver in
# the SMT programming language.
# --------------------------------------------------------

class Z3:

	def __init__(self,sb,sw):
		file = open("c11","w")
		contents = ""
		
		# declare function for sb's
		contents+=self.function("sb",["Bool","Bool"],"Bool")

		# declare function for sw's
		contents+=self.function("sw",["Bool","Bool"],"Bool")
		
		# creating the constants 
		for thread in sb:
			for edge in thread:
				contents+= self.constant(edge,"Bool")

		# make the sb assertions
		for thread in sb:
			for i in range(len(thread)):
				if i!=len(thread)-1:
					left = "(sb "+thread[i]+" "+thread[i+1]+")"
					contents+=self.fact("=",left,"true")

		# make the sw assertions
		for thread in sw:
			for i in range(len(thread)):
				if i!=len(thread)-1:
					left = "(sw "+thread[i]+" "+thread[i+1]+")"
					contents+=self.fact("=",left,"true")
		
		contents+="(check-sat)\n(get-model)"
		file.write(contents)

	# to declare and define a function
	def function(self,name,inputs,output):
		fn = "(declare-fun "+name+" ("
		for val in inputs:
			fn+=val+" "
		fn+=") "+output+")\n"
		return fn

	# to declare and define a constant
	def constant(self,name,dtype):
		cnst = "(declare-const "+name+" "+dtype+")\n"
		return cnst

	# to add assertions/facts
	def fact(self,operator,left,right):
		assertion = "(assert ("+operator+" "+left+" "+right+"))\n"
		return assertion