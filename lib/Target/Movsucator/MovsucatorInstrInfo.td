//===-- MovsucatorInstrInfo.td - Instruction information -*- tablegen -*-===//

include "MovsucatorInstrFormats.td"

// Memory operand definitions
def addr : Operand<iPTR>;
def memop : Operand<i32>;

// Define instruction classes
def MOV_rr : MovFormat<0b000000,
  (outs GPR:$dst), (ins GPR:$src),
  "mov $dst, $src",
  [(set GPR:$dst, GPR:$src)]> {
}

// Special MOV instructions for handling memory operations
def MOV_rm : MovFormat<0b000010,
  (outs GPR:$dst), (ins addr:$src),
  "mov $dst, [$src]",
  [(set GPR:$dst, (load iPTR:$src))]> {
}

def MOV_mr : MovFormat<0b000011,
  (outs), (ins addr:$dst, GPR:$src),
  "mov [$dst], $src",
  [(store GPR:$src, iPTR:$dst)]> {
}

// Immediate MOV instruction
def MOV_ri : MovFormat<0b000001,
  (outs GPR:$dst), (ins i32imm:$imm),
  "mov $dst, $imm",
  [(set GPR:$dst, imm:$imm)]> {
}