# memory orders
SEQ_CST = "seq_cst"
REL = "release"
ACQ = "acquire"
ACQ_REL = "acq_rel"
RLX = "relaxed"

# instruction types
RMW = "rmw"
WRITE = "write"
INIT = "init"
READ = "read"
FENCE = "fence"

# thread operations
JOIN = "join"
CREATE = "create"
START = "start"
FINISH = "finish"

# instruction indices
S_NO = 0					# instruction serial number
T_NO = 1					# instruction thread number
TYPE = 2					# instruction type (read/write etc)
MO	= 3						# instruction memory order (seq_cst etc)
ADDR = 4					# memory address where instr is stored
VALUE = 5					# value of variable/thread in the instruction
RF = 6						# rf serial no
LINE_NO = 7					# line number in source code

# files, folders
Z3_FILE = "compute_fences"
CDS_FOLDER = "../model-checker/cds-checker"
CDS_TEST_FOLDER = "../model-checker/cds-checker/test"