//===-- MovsucatorInstrInfo.td - Instruction information -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Instruction Definitions
//===----------------------------------------------------------------------===//

// Basic register-to-register MOV
def MOV_rr : MovFormat<0b000000,
  (outs GPR:$dst), (ins GPR:$src),
  "mov $dst, $src",
  [(set GPR:$dst, GPR:$src)]>;

// Memory load MOV
def MOV_rm : MovFormat<0b000010,
  (outs GPR:$dst), (ins addr:$src),
  "mov $dst, [$src]",
  [(set GPR:$dst, (load iPTR:$src))]>;

// Memory store MOV
def MOV_mr : MovFormat<0b000011,
  (outs), (ins addr:$dst, GPR:$src),
  "mov [$dst], $src",
  [(store GPR:$src, iPTR:$dst)]>;

// Immediate MOV instruction
def MOV_ri : MovFormat<0b000001,
  (outs GPR:$dst), (ins i32imm:$imm),
  "mov $dst, $imm",
  [(set GPR:$dst, imm:$imm)]> {
}