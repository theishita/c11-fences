### Pre-requisites
* Linux environment
* Python3.x
* pip / anaconda installer for python libraries
* g++ compiler

## Installation
* Clone this repository and navigate inside it:
```
git clone https://github.com/theishita/c11-fences && cd c11-fences
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
The ouput file will be generated in the source path with a `_fence` appended to its name.

### Input file requirements
(check "tool description" section in the paper)

For detailed instructions with examples and explanations, check "Tool Description" section in the paper.
