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