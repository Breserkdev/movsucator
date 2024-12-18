//===-- MovsucatorInstrFormats.td - Instruction formats -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Instruction Format Definitions
//===----------------------------------------------------------------------===//

// Base instruction class
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

// Format for MOV instructions
class MovFormat<bits<6> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MovsucatorInst<outs, ins, asmstr, pattern> {
  bits<6> Opcode = opcode;
  
  let Inst{31-26} = opcode;
}

include "MovsucatorInstrInfo.td"