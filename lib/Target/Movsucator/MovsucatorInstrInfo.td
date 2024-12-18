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

// Pseudo instruction for addition using MOV
def MOV_ADD_INST : MovFormat<0b000100,
  (outs GPR:$dst), (ins GPR:$src),
  "mov.add $dst, $src",
  [(set GPR:$dst, (add GPR:$dst, GPR:$src))]> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// MOV-based addition expansion patterns
def : Pat<(add GPR:$src1, GPR:$src2),
          (MOV_ADD_INST GPR:$src1, GPR:$src2)>;

// Patterns for MOV-based arithmetic operations
def : Pat<(MovsucatorISD::MOV_ADD GPR:$src1, GPR:$src2),
          (MOV_ADD_INST GPR:$src1, GPR:$src2)>;

def : Pat<(MovsucatorISD::MOV_NEG GPR:$src),
          (MOV_NEG GPR:$src)>;

def : Pat<(MovsucatorISD::MOV_AND GPR:$src1, GPR:$src2),
          (MOV_AND GPR:$src1, GPR:$src2)>;

def : Pat<(MovsucatorISD::MOV_OR GPR:$src1, GPR:$src2),
          (MOV_OR GPR:$src1, GPR:$src2)>;

def : Pat<(MovsucatorISD::MOV_XOR GPR:$src1, GPR:$src2),
          (MOV_XOR GPR:$src1, GPR:$src2)>;

// Patterns for MOV-based comparison operations
def : Pat<(MovsucatorISD::MOV_CMP GPR:$src1, GPR:$src2),
          (MOV_CMP GPR:$src1, GPR:$src2)>;

// Pattern for conditional branch
def : Pat<(MovsucatorISD::MOV_BR GPR:$cond, bb:$dest),
          (MOV_BR GPR:$cond, bb:$dest)>;

// Memory operation patterns
def : Pat<(MovsucatorISD::MOV_LOAD addr:$addr),
          (MOV_rm addr:$addr)>;

def : Pat<(MovsucatorISD::MOV_STORE GPR:$src, addr:$addr),
          (MOV_mr addr:$addr, GPR:$src)>;

// Frame setup/destroy patterns
def : Pat<(MovsucatorCall tglobaladdr:$dst),
          (MOV_CALL tglobaladdr:$dst)>;

def : Pat<(MovsucatorRet),
          (MOV_RET)>;

// Frame cleanup pattern
def MOV_CLEANUP : MovFormat<0b000101,
  (outs), (ins),
  "mov.cleanup",
  []> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// MOV Sequence Implementations
//===----------------------------------------------------------------------===//

// Addition using MOV sequences
def MovAdd : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                             [(set GPR:$dst, (MovsucatorISD::MOV_ADD GPR:$src1, GPR:$src2))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Negation using MOV sequences
def MovNegate : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src),
                                [(set GPR:$dst, (MovsucatorISD::MOV_NEG GPR:$src))]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// AND using MOV sequences
def MovAnd : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                             [(set GPR:$dst, (MovsucatorISD::MOV_AND GPR:$src1, GPR:$src2))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// OR using MOV sequences
def MovOr : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                            [(set GPR:$dst, (MovsucatorISD::MOV_OR GPR:$src1, GPR:$src2))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// XOR using MOV sequences
def MovXor : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                             [(set GPR:$dst, (MovsucatorISD::MOV_XOR GPR:$src1, GPR:$src2))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Compare operation using MOV sequences
def MovCompare : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src1, GPR:$src2),
                                 [(set GPR:$dst, (MovsucatorISD::MOV_CMP GPR:$src1, GPR:$src2))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Conditional branch using MOV sequences
def MovBranch : MovsucatorPseudo<(outs), (ins GPR:$cond, bb:$dest),
                                [(MovsucatorISD::MOV_BR GPR:$cond, bb:$dest)]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Optimized sequences for common operations
def MovIncrement : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src),
                                   [(set GPR:$dst, (add GPR:$src, 1))]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

def MovDecrement : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src),
                                   [(set GPR:$dst, (add GPR:$src, -1))]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

def MovZero : MovsucatorPseudo<(outs GPR:$dst), (ins),
                              [(set GPR:$dst, 0)]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

def MovAllOnes : MovsucatorPseudo<(outs GPR:$dst), (ins),
                                 [(set GPR:$dst, -1)]> {
  let Defs = [A_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// Shift and Rotate Operations
//===----------------------------------------------------------------------===//

// Left shift using MOV sequences
def MovSHL : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src, i32imm:$shamt),
                             [(set GPR:$dst, (shl GPR:$src, imm:$shamt))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Logical right shift using MOV sequences
def MovLSHR : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src, i32imm:$shamt),
                              [(set GPR:$dst, (srl GPR:$src, imm:$shamt))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Arithmetic right shift using MOV sequences
def MovASHR : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src, i32imm:$shamt),
                              [(set GPR:$dst, (sra GPR:$src, imm:$shamt))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Rotate left using MOV sequences
def MovROL : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src, i32imm:$shamt),
                             [(set GPR:$dst, (rotl GPR:$src, imm:$shamt))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Rotate right using MOV sequences
def MovROR : MovsucatorPseudo<(outs GPR:$dst), (ins GPR:$src, i32imm:$shamt),
                             [(set GPR:$dst, (rotr GPR:$src, imm:$shamt))]> {
  let Defs = [A_Reg, B_Reg];
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// Complex Operation Patterns
//===----------------------------------------------------------------------===//

// Optimize shifts by 1 to add
def : Pat<(shl GPR:$src, 1), (MovAdd GPR:$src, GPR:$src)>;

// Optimize multiplication by 2^n to shift
def : Pat<(mul GPR:$src, (i32 2)), (MovSHL GPR:$src, 1)>;
def : Pat<(mul GPR:$src, (i32 4)), (MovSHL GPR:$src, 2)>;
def : Pat<(mul GPR:$src, (i32 8)), (MovSHL GPR:$src, 3)>;
def : Pat<(mul GPR:$src, (i32 16)), (MovSHL GPR:$src, 4)>;

// Optimize division by 2^n to shift
def : Pat<(udiv GPR:$src, (i32 2)), (MovLSHR GPR:$src, 1)>;
def : Pat<(udiv GPR:$src, (i32 4)), (MovLSHR GPR:$src, 2)>;
def : Pat<(udiv GPR:$src, (i32 8)), (MovLSHR GPR:$src, 3)>;
def : Pat<(udiv GPR:$src, (i32 16)), (MovLSHR GPR:$src, 4)>;

// Optimize signed division by 2^n to arithmetic shift
def : Pat<(sdiv GPR:$src, (i32 2)), (MovASHR GPR:$src, 1)>;
def : Pat<(sdiv GPR:$src, (i32 4)), (MovASHR GPR:$src, 2)>;
def : Pat<(sdiv GPR:$src, (i32 8)), (MovASHR GPR:$src, 3)>;
def : Pat<(sdiv GPR:$src, (i32 16)), (MovASHR GPR:$src, 4)>;

// Optimize bit extraction
def : Pat<(and (srl GPR:$src, imm:$shamt), (i32 1)),
          (MovBitTest GPR:$src, imm:$shamt)>;

// Optimize bit insertion
def : Pat<(or (and GPR:$val, (not (shl 1, imm:$bitno))),
              (shl (and GPR:$bit, 1), imm:$bitno)),
          (MovBitInsert GPR:$val, GPR:$bit, imm:$bitno)>;

//===----------------------------------------------------------------------===//
// Optimization Patterns
//===----------------------------------------------------------------------===//

// Optimize increment/decrement
def : Pat<(add GPR:$src, 1), (MovIncrement GPR:$src)>;
def : Pat<(add GPR:$src, -1), (MovDecrement GPR:$src)>;

// Optimize AND with 0 to mov zero
def : Pat<(and GPR:$src, 0), (MovZero)>;

// Optimize OR with -1 to mov all ones
def : Pat<(or GPR:$src, -1), (MovAllOnes)>;

// Optimize XOR with self to zero
def : Pat<(xor GPR:$src, GPR:$src), (MovZero)>;

// Optimize ADD with 0 to simple move
def : Pat<(add GPR:$src, 0), (MOV_rr GPR:$src)>;

// Optimize AND with -1 to simple move
def : Pat<(and GPR:$src, -1), (MOV_rr GPR:$src)>;

// Optimize OR with 0 to simple move
def : Pat<(or GPR:$src, 0), (MOV_rr GPR:$src)>;

// Optimize comparison with 0
def : Pat<(setcc GPR:$src, 0, SETEQ), (MovCompareZero GPR:$src)>;
def : Pat<(setcc GPR:$src, 0, SETNE), (MovCompareNonZero GPR:$src)>;

// Optimize conditional moves
def : Pat<(select GPR:$cond, GPR:$true, GPR:$false),
          (MovSelect GPR:$cond, GPR:$true, GPR:$false)>;