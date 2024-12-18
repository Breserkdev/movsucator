//===-- MovsucatorBase.td - Common definitions for Movsucator --*- tablegen -*-===//

include "llvm/Target/Target.td"

// No need to include RuntimeLibcalls.td here since Target.td already includes it

//===----------------------------------------------------------------------===//
// Basic characteristics
//===----------------------------------------------------------------------===//

def MovsucatorInstrInfo : InstrInfo;

def MovsucatorAsmWriter : AsmWriter {
  let AsmWriterClassName  = "InstPrinter";
}

//===----------------------------------------------------------------------===//
// Operand definitions
//===----------------------------------------------------------------------===//

def i32imm_op : Operand<i32>;
def i32mem_op : Operand<i32> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops GPR, i32imm_op);
}

def ri_op : AsmOperandClass {
  let Name = "RegImm";
  let ParserMethod = "parseRegOrImm";
}

def mem_op : AsmOperandClass {
  let Name = "Mem";
  let ParserMethod = "parseMemOperand";
}

//===----------------------------------------------------------------------===//
// Base instruction class
//===----------------------------------------------------------------------===//

class MovsucatorInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;  // All instructions are 32-bit
  let Namespace = "Movsucator";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
  let Size = 4;
}