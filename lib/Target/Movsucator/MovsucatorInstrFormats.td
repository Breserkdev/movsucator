//===-- MovsucatorInstrFormats.td - Instruction formats -*- tablegen -*-===//

// Format for MOV instructions
class MovFormat<bits<6> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MovsucatorInst<outs, ins, asmstr, pattern> {
  let Inst{31-26} = opcode;
}

// Basic MOV instruction format
def MOV_rr : MovFormat<0b000000,
  (outs GPR:$dst),
  (ins GPR:$src),
  "mov $dst, $src",
  [(set GPR:$dst, GPR:$src)]>;

// MOV immediate format
def MOV_ri : MovFormat<0b000001,
  (outs GPR:$dst),
  (ins i32imm:$imm),
  "mov $dst, $imm",
  [(set GPR:$dst, imm:$imm)]>;

// MOV memory load format
def MOV_rm : MovFormat<0b000010,
  (outs GPR:$dst),
  (ins mem:$src),
  "mov $dst, [$src]",
  [(set GPR:$dst, (load addr:$src))]>;

// MOV memory store format
def MOV_mr : MovFormat<0b000011,
  (outs),
  (ins mem:$dst, GPR:$src),
  "mov [$dst], $src",
  [(store GPR:$src, addr:$dst)]>;