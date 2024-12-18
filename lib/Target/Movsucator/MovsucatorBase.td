//===-- MovsucatorBase.td - Common definitions for Movsucator --*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Basic Definitions
//===----------------------------------------------------------------------===//

def MovsucatorAsmWriter : AsmWriter {
  let AsmWriterClassName  = "InstPrinter";
}

//===----------------------------------------------------------------------===//
// Operand definitions
//===----------------------------------------------------------------------===//

def i32imm_op : Operand<i32>;
def addr : Operand<iPTR>;
def memop : Operand<i32>;

//===----------------------------------------------------------------------===//
// Custom nodes for MOV-based operations
//===----------------------------------------------------------------------===//

def MovsucatorISD {
}

def MOV : SDNode<"MovsucatorISD::MOV", SDTIntBinOp>;
def MOV_ADD : SDNode<"MovsucatorISD::MOV_ADD", SDTIntBinOp>;
def MOV_NEG : SDNode<"MovsucatorISD::MOV_NEG", SDTIntUnaryOp>;
def MOV_AND : SDNode<"MovsucatorISD::MOV_AND", SDTIntBinOp>;
def MOV_OR : SDNode<"MovsucatorISD::MOV_OR", SDTIntBinOp>;
def MOV_XOR : SDNode<"MovsucatorISD::MOV_XOR", SDTIntBinOp>;

// Memory operations
def MOV_LOAD : SDNode<"MovsucatorISD::MOV_LOAD", SDTIntUnaryOp>;
def MOV_STORE : SDNode<"MovsucatorISD::MOV_STORE", SDTIntBinOp>;