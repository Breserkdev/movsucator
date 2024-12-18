//===-- MovsucatorInstrFormats.td - Instruction formats -*- tablegen -*-===//

// Base class for all Movsucator instructions
class MovsucatorInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;
  let Namespace = "Movsucator";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// Format for MOV instructions
class MovFormat<bits<6> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MovsucatorInst<outs, ins, asmstr, pattern> {
  let Inst{31-26} = opcode;
  let Inst{25-0} = 0;  // Reserved for operand encoding
}