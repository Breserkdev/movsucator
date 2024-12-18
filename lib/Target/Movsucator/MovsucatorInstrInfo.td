//===-- MovsucatorInstrInfo.td - Instruction information -*- tablegen -*-===//

include "MovsucatorInstrFormats.td"

// Define instruction classes
def MOV_rr : MovFormat<0b000000,
  (outs GPR:$dst), (ins GPR:$src),
  "mov $dst, $src",
  [(set GPR:$dst, GPR:$src)]> {
  let Sched = [];
}

// Special MOV instructions for handling memory operations
def MOV_rm : MovFormat<0b000010,
  (outs GPR:$dst), (ins memri:$src),
  "mov $dst, [$src]",
  [(set GPR:$dst, (load addr:$src))]> {
  let Sched = [];
}

def MOV_mr : MovFormat<0b000011,
  (outs), (ins memri:$dst, GPR:$src),
  "mov [$dst], $src",
  [(store GPR:$src, addr:$dst)]> {
  let Sched = [];
}

// Immediate MOV instruction
def MOV_ri : MovFormat<0b000001,
  (outs GPR:$dst), (ins i32imm:$imm),
  "mov $dst, $imm",
  [(set GPR:$dst, imm:$imm)]> {
  let Sched = [];
}