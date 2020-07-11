### Pre-requisites
* Python3.x
* pip / anaconda installer for python libraries

## Installation
* Clone the repository
```
git clone https://github.com/theishita/c11-fences && cd c11-fences
```
* Add CDS model checker:
```
cd code
git clone git://demsky.eecs.uci.edu/model-checker.git
```
* Install required python libraries:<br/>
```
pip install networkx ast operator shlex
```
* Install Z3:<br/>
```
sudo apt update
sudo apt install z3
```

## Usage
To add fences to your C11 program with assertions, run the following command inside `code` folder:<br />
```
python3 main.py -f <your file path>
```
For examples, check benchmarks folder. <br />
The ouput file will be generated in the source path with an extension `_fence` added to its name.

### Input file requirements
1. Each atomic operation needs to be on a separate line unless it is an argument inside (say) an if statement with boolean operators.
2. Each atomic variable used in the program needs to be initialized in the main/parent thread.
3. While writing a "store" operation, there needs to be a space character after the comma after the value is written.
4. he thread functions need to be defined in the same order as they were called in the main/parent thread before the main thread and there can be no other extra functions apart from the thread functions and the main function.
5. The starting bracket for each function definition, loop call, or if-else statement needs to be in the same line as the function name.
6. Add dummy variable store operations above RMW instructions, repeated instructions inside threads and store instructions with input values as variables.
7. "Thread create" statements inside loops need to be unravelled and their functions need to be re-written as many times as they are called, with different names.
8. 'IF' and 'FOR' statements always need to be bracketed in case there is a fence insertion taking place inside.

For detailed instructions with examples and explanations, check "Tool Description" section in the paper.
