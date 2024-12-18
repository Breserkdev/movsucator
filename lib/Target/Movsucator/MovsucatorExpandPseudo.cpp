//===-- MovsucatorExpandPseudo.cpp - Expand Pseudo Instructions -*- C++ -*-===//

#include "MovsucatorInstrInfo.h"
#include "MovsucatorSubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "movsucator-pseudo-expand"

namespace {
class MovsucatorExpandPseudo : public MachineFunctionPass {
public:
  static char ID;
  MovsucatorExpandPseudo() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  const MovsucatorInstrInfo *TII;
  bool expandMOV_ADD(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_NEG(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_AND(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_OR(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_XOR(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_CMP(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_BR(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_INCREMENT(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandMOV_DECREMENT(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI);
  bool expandShiftLeft(MachineInstr &MI);
  bool expandShiftRight(MachineInstr &MI, bool isArithmetic);
  bool expandRotate(MachineInstr &MI, bool isLeft);
};
} // end anonymous namespace

char MovsucatorExpandPseudo::ID = 0;

bool MovsucatorExpandPseudo::runOnMachineFunction(MachineFunction &MF) {
  TII = static_cast<const MovsucatorInstrInfo *>(
      MF.getSubtarget().getInstrInfo());
  bool Modified = false;

  for (auto &MBB : MF) {
    for (auto MBBI = MBB.begin(); MBBI != MBB.end();) {
      MachineInstr &MI = *MBBI;
      ++MBBI;
      
      switch (MI.getOpcode()) {
      default:
        break;
      case Movsucator::MovAdd:
        Modified |= expandMOV_ADD(MBB, MI);
        break;
      case Movsucator::MovNegate:
        Modified |= expandMOV_NEG(MBB, MI);
        break;
      case Movsucator::MovAnd:
        Modified |= expandMOV_AND(MBB, MI);
        break;
      case Movsucator::MovOr:
        Modified |= expandMOV_OR(MBB, MI);
        break;
      case Movsucator::MovXor:
        Modified |= expandMOV_XOR(MBB, MI);
        break;
      case Movsucator::MovCompare:
        Modified |= expandMOV_CMP(MBB, MI);
        break;
      case Movsucator::MovBranch:
        Modified |= expandMOV_BR(MBB, MI);
        break;
      case Movsucator::MovIncrement:
        Modified |= expandMOV_INCREMENT(MBB, MI);
        break;
      case Movsucator::MovDecrement:
        Modified |= expandMOV_DECREMENT(MBB, MI);
        break;
      case Movsucator::MovSHL:
        Modified |= expandShiftLeft(MI);
        break;
      case Movsucator::MovLSHR:
        Modified |= expandShiftRight(MI, false);
        break;
      case Movsucator::MovASHR:
        Modified |= expandShiftRight(MI, true);
        break;
      case Movsucator::MovROL:
        Modified |= expandRotate(MI, true);
        break;
      case Movsucator::MovROR:
        Modified |= expandRotate(MI, false);
        break;
      }
    }
  }
  return Modified;
}

bool MovsucatorExpandPseudo::expandMOV_ADD(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI) {
  // Implement ADD using MOV sequences:
  // 1. MOV result, src1
  // 2. MOV temp, src2
  // 3. Use bit manipulation with MOV to add temp to result
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register Src1Reg = MI.getOperand(1).getReg();
  Register Src2Reg = MI.getOperand(2).getReg();
  
  // First, move src1 to result
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), DstReg)
      .addReg(Src1Reg);
      
  // Move src2 to temp register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(Src2Reg);
      
  // Perform addition using bit manipulation (implemented in hardware)
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ADD), DstReg)
      .addReg(Movsucator::A_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_NEG(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI) {
  // Implement NEG using MOV sequences:
  // 1. MOV result, 0
  // 2. MOV temp, src
  // 3. Use bit manipulation with MOV to subtract from 0
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  
  // First, move 0 to result
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ri), DstReg)
      .addImm(0);
      
  // Move src to temp register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(SrcReg);
      
  // Perform negation using bit manipulation (implemented in hardware)
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_NEG), DstReg)
      .addReg(Movsucator::A_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_AND(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI) {
  // Implement AND using MOV sequences
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register Src1Reg = MI.getOperand(1).getReg();
  Register Src2Reg = MI.getOperand(2).getReg();
  
  // Move operands to temporary registers
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(Src1Reg);
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::B_Reg)
      .addReg(Src2Reg);
      
  // Perform AND using bit manipulation (implemented in hardware)
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_AND), DstReg)
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::B_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_OR(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator MBBI) {
  // Implement OR using MOV sequences:
  // 1. Move operands to temporary registers
  // 2. Use bit manipulation to perform OR
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register Src1Reg = MI.getOperand(1).getReg();
  Register Src2Reg = MI.getOperand(2).getReg();
  
  // Move operands to temporary registers
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(Src1Reg);
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::B_Reg)
      .addReg(Src2Reg);
      
  // Perform OR using bit manipulation
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_OR), DstReg)
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::B_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_XOR(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI) {
  // Implement XOR using MOV sequences:
  // 1. Move operands to temporary registers
  // 2. Use bit manipulation to perform XOR
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register Src1Reg = MI.getOperand(1).getReg();
  Register Src2Reg = MI.getOperand(2).getReg();
  
  // Move operands to temporary registers
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(Src1Reg);
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::B_Reg)
      .addReg(Src2Reg);
      
  // Perform XOR using bit manipulation
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_XOR), DstReg)
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::B_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_CMP(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI) {
  // Implement CMP using MOV sequences:
  // 1. Move operands to temporary registers
  // 2. Subtract second from first using NEG and ADD
  // 3. Set flags based on result
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register Src1Reg = MI.getOperand(1).getReg();
  Register Src2Reg = MI.getOperand(2).getReg();
  
  // Move first operand to result register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), DstReg)
      .addReg(Src1Reg);
      
  // Move second operand to temp register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(Src2Reg);
      
  // Negate second operand
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_NEG), Movsucator::A_Reg)
      .addReg(Movsucator::A_Reg);
      
  // Add negated second operand to first (effectively subtracting)
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ADD), DstReg)
      .addReg(Movsucator::A_Reg);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_BR(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator MBBI) {
  // Implement conditional branch using MOV sequences:
  // 1. Move condition to temp register
  // 2. Compare with zero
  // 3. Branch based on result
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register CondReg = MI.getOperand(0).getReg();
  MachineBasicBlock *TargetBB = MI.getOperand(1).getMBB();
  
  // Move condition to A register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), Movsucator::A_Reg)
      .addReg(CondReg);
      
  // Compare with zero (implemented in hardware)
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_CMP), Movsucator::A_Reg)
      .addReg(Movsucator::A_Reg)
      .addImm(0);
      
  // Branch if not zero
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_BR_NZ))
      .addMBB(TargetBB);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_INCREMENT(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MBBI) {
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  
  // Move source to destination
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), DstReg)
      .addReg(SrcReg);
      
  // Add 1 using optimized sequence
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ADD), DstReg)
      .addImm(1);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandMOV_DECREMENT(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MBBI) {
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();
  
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  
  // Move source to destination
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr), DstReg)
      .addReg(SrcReg);
      
  // Subtract 1 using optimized sequence
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ADD), DstReg)
      .addImm(-1);
      
  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandShiftLeft(MachineInstr &MI) {
  // Get operands
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  unsigned ShAmt = MI.getOperand(2).getImm();
  
  // For shift amount of 0, just move the value
  if (ShAmt == 0) {
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(DstReg)
      .addReg(SrcReg);
    MI.eraseFromParent();
    return true;
  }

  // Use A_Reg as temporary
  Register TmpReg = Movsucator::A_Reg;
  
  // First copy source to temporary
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(TmpReg)
    .addReg(SrcReg);

  // Perform shift by repeatedly adding the value to itself
  for (unsigned i = 0; i < ShAmt; ++i) {
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(TmpReg);
  }

  // Move result to destination
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(DstReg)
    .addReg(TmpReg);

  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandShiftRight(MachineInstr &MI, bool isArithmetic) {
  // Get operands
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  unsigned ShAmt = MI.getOperand(2).getImm();
  
  // For shift amount of 0, just move the value
  if (ShAmt == 0) {
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(DstReg)
      .addReg(SrcReg);
    MI.eraseFromParent();
    return true;
  }

  // Use A_Reg and B_Reg as temporaries
  Register TmpReg = Movsucator::A_Reg;
  Register MaskReg = Movsucator::B_Reg;
  
  // First copy source to temporary
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(TmpReg)
    .addReg(SrcReg);

  // Create mask for logical right shift
  uint32_t Mask = ~((1U << ShAmt) - 1);
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(MaskReg)
    .addImm(Mask);

  // For arithmetic shift, preserve sign bit
  if (isArithmetic) {
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(MaskReg);
  } else {
    // For logical shift, just clear upper bits
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(MaskReg);
  }

  // Move result to destination
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(DstReg)
    .addReg(TmpReg);

  MI.eraseFromParent();
  return true;
}

bool MovsucatorExpandPseudo::expandRotate(MachineInstr &MI, bool isLeft) {
  // Get operands
  Register DstReg = MI.getOperand(0).getReg();
  Register SrcReg = MI.getOperand(1).getReg();
  unsigned RotAmt = MI.getOperand(2).getImm() & 0x1F; // Normalize to 0-31
  
  // For rotate amount of 0, just move the value
  if (RotAmt == 0) {
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(DstReg)
      .addReg(SrcReg);
    MI.eraseFromParent();
    return true;
  }

  // Use A_Reg and B_Reg as temporaries
  Register TmpReg = Movsucator::A_Reg;
  Register ShiftReg = Movsucator::B_Reg;
  
  // First copy source to temporary
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(TmpReg)
    .addReg(SrcReg);

  if (isLeft) {
    // Rotate left: ROL(x, n) = (x << n) | (x >> (32-n))
    uint32_t LeftMask = (1U << RotAmt) - 1;
    uint32_t RightMask = ~LeftMask;
    
    // Shift left part
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(ShiftReg)
      .addImm(LeftMask);
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(ShiftReg);

    // Shift right part
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(ShiftReg)
      .addImm(RightMask);
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(ShiftReg);
  } else {
    // Rotate right: ROR(x, n) = (x >> n) | (x << (32-n))
    uint32_t RightMask = ~((1U << (32 - RotAmt)) - 1);
    uint32_t LeftMask = ~RightMask;
    
    // Shift right part
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(ShiftReg)
      .addImm(RightMask);
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(ShiftReg);

    // Shift left part
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(ShiftReg)
      .addImm(LeftMask);
    BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
      .addReg(TmpReg)
      .addReg(TmpReg)
      .addReg(ShiftReg);
  }

  // Move result to destination
  BuildMI(*MI.getParent(), MI, MI.getDebugLoc(), TII->get(Movsucator::MOV))
    .addReg(DstReg)
    .addReg(TmpReg);

  MI.eraseFromParent();
  return true;
}

INITIALIZE_PASS(MovsucatorExpandPseudo, DEBUG_TYPE,
                "Movsucator Pseudo Instruction Expansion", false, false)

FunctionPass *llvm::createMovsucatorExpandPseudoPass() {
  return new MovsucatorExpandPseudo();
}
