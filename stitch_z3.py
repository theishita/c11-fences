# --------------------------------------------------------
# Creates functions and realtions for the Z3 sat solver in
# the SMT programming language.
# --------------------------------------------------------

class Z3:

	def __init__(self,sb,sw):
		file = open("c11","w")
		contents = ""
				
		n = 0																# n = number of total fences
		# counting the number of constants 
		for thread in sb:
			for edge in thread:
				n+=1
		const_dtype = "(_ BitVec "+str(n)+")"								# the fences datatypes are (BitVector n) where n is the size

		# creating the constants 
		for thread in sb:
			for edge in thread:
				contents+= self.constant(edge,const_dtype)

		# declare function for sb's
		contents+=self.function("sb",[const_dtype,const_dtype],"Bool")

		# declare function for sw's
		contents+=self.function("sw",[const_dtype,const_dtype],"Bool")
		
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