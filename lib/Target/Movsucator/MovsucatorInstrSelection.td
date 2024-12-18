//===-- MovsucatorInstrSelection.td - Instruction Selection Rules -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Instruction Selection Patterns
//===----------------------------------------------------------------------===//

// ADD using MOV sequences
def : Pat<(MovsucatorISD::MOV_ADD GPR:$src1, GPR:$src2), (MovAdd GPR:$src1, GPR:$src2)>;

// SUB using MOV sequences (a - b = a + (-b))
def : Pat<(MovsucatorISD::MOV_NEG GPR:$src), (MovNegate GPR:$src)>;

// AND using MOV sequences
def : Pat<(MovsucatorISD::MOV_AND GPR:$src1, GPR:$src2), (MovAnd GPR:$src1, GPR:$src2)>;

// OR using MOV sequences
def : Pat<(MovsucatorISD::MOV_OR GPR:$src1, GPR:$src2), (MovOr GPR:$src1, GPR:$src2)>;

// XOR using MOV sequences
def : Pat<(MovsucatorISD::MOV_XOR GPR:$src1, GPR:$src2), (MovXor GPR:$src1, GPR:$src2)>;

// Compare operations
def : Pat<(MovsucatorISD::MOV_CMP GPR:$src1, GPR:$src2), (MovCompare GPR:$src1, GPR:$src2)>;

// Conditional branch
def : Pat<(MovsucatorISD::MOV_BR GPR:$cond, bb:$dest), (MovBranch GPR:$cond, bb:$dest)>;

// Optimization patterns for common sequences
def : Pat<(MovsucatorISD::MOV_ADD GPR:$src, (i32 1)), (MovIncrement GPR:$src)>;
def : Pat<(MovsucatorISD::MOV_ADD GPR:$src, (i32 -1)), (MovDecrement GPR:$src)>;
def : Pat<(MovsucatorISD::MOV_AND GPR:$src, (i32 0)), (MovZero GPR:$src)>;
def : Pat<(MovsucatorISD::MOV_OR GPR:$src, (i32 -1)), (MovAllOnes GPR:$src)>;

// Memory access patterns
def : Pat<(MovsucatorISD::MOV_LOAD addr:$addr), (MovLoad addr:$addr)>;
def : Pat<(MovsucatorISD::MOV_STORE GPR:$src, addr:$addr), (MovStore addr:$addr, GPR:$src)>;
