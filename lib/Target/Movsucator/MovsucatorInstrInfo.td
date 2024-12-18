//===-- MovsucatorInstrInfo.td - Instruction information -*- tablegen -*-===//

include "MovsucatorInstrFormats.td"

// Define instruction classes
def MOV : MovsucatorInst<(outs GPR:$dst), (ins GPR:$src),
                         "mov $dst, $src",
                         [(set GPR:$dst, GPR:$src)]>;

// Special MOV instructions for handling memory operations
def MOV_load : MovsucatorInst<(outs GPR:$dst), (ins memri:$src),
                             "mov $dst, [$src]",
                             [(set GPR:$dst, (load addr:$src))]>;

def MOV_store : MovsucatorInst<(outs), (ins memri:$dst, GPR:$src),
                              "mov [$dst], $src",
                              [(store GPR:$src, addr:$dst)]>;

// Immediate MOV instruction
def MOVi : MovsucatorInst<(outs GPR:$dst), (ins i32imm:$imm),
                          "mov $dst, $imm",
                          [(set GPR:$dst, imm:$imm)]>;