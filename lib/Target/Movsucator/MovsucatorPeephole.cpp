//===-- MovsucatorPeephole.cpp - Peephole Optimizations -*- C++ -*-===//

#include "MovsucatorInstrInfo.h"
#include "MovsucatorSubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "movsucator-peephole"

namespace {
class MovsucatorPeephole : public MachineFunctionPass {
public:
  static char ID;
  MovsucatorPeephole() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  const MovsucatorInstrInfo *TII;
  MachineRegisterInfo *MRI;

  bool optimizeBlock(MachineBasicBlock &MBB);
  bool optimizeLoadStore(MachineBasicBlock::iterator &I);
  bool optimizeArithmetic(MachineBasicBlock::iterator &I);
  bool optimizeBranches(MachineBasicBlock::iterator &I);
  bool mergeMoves(MachineBasicBlock::iterator &I);
  bool eliminateDeadMoves(MachineBasicBlock::iterator &I);
};
} // end anonymous namespace

char MovsucatorPeephole::ID = 0;

bool MovsucatorPeephole::runOnMachineFunction(MachineFunction &MF) {
  TII = static_cast<const MovsucatorInstrInfo *>(
      MF.getSubtarget().getInstrInfo());
  MRI = &MF.getRegInfo();
  bool Changed = false;

  for (auto &MBB : MF)
    Changed |= optimizeBlock(MBB);

  return Changed;
}

bool MovsucatorPeephole::optimizeBlock(MachineBasicBlock &MBB) {
  bool Changed = false;
  for (auto I = MBB.begin(); I != MBB.end();) {
    auto Next = std::next(I);
    if (optimizeLoadStore(I) || optimizeArithmetic(I) ||
        optimizeBranches(I) || mergeMoves(I) || eliminateDeadMoves(I))
      Changed = true;
    I = Next;
  }
  return Changed;
}

bool MovsucatorPeephole::optimizeLoadStore(MachineBasicBlock::iterator &I) {
  // Pattern 1: Load followed by immediate store to same location
  // mov r1, [addr]
  // mov [addr], r1
  // =>
  // (eliminate both instructions)
  if (I->getOpcode() == Movsucator::MOV_rm) {
    auto Next = std::next(I);
    if (Next != I->getParent()->end() &&
        Next->getOpcode() == Movsucator::MOV_mr) {
      // Check if addresses match and registers match
      if (I->getOperand(1).getImm() == Next->getOperand(0).getImm() &&
          I->getOperand(0).getReg() == Next->getOperand(1).getReg()) {
        I->eraseFromParent();
        Next->eraseFromParent();
        return true;
      }
    }
  }
  return false;
}

bool MovsucatorPeephole::optimizeArithmetic(MachineBasicBlock::iterator &I) {
  // Pattern 1: ADD with 0
  // mov r1, r2
  // mov.add r1, 0
  // =>
  // mov r1, r2
  if (I->getOpcode() == Movsucator::MOV_rr) {
    auto Next = std::next(I);
    if (Next != I->getParent()->end() &&
        Next->getOpcode() == Movsucator::MOV_ADD &&
        Next->getOperand(1).isImm() &&
        Next->getOperand(1).getImm() == 0) {
      Next->eraseFromParent();
      return true;
    }
  }

  // Pattern 2: Double increment/decrement
  // mov.add r1, 1
  // mov.add r1, 1
  // =>
  // mov.add r1, 2
  if (I->getOpcode() == Movsucator::MOV_ADD) {
    auto Next = std::next(I);
    if (Next != I->getParent()->end() &&
        Next->getOpcode() == Movsucator::MOV_ADD &&
        I->getOperand(0).getReg() == Next->getOperand(0).getReg() &&
        I->getOperand(1).isImm() && Next->getOperand(1).isImm()) {
      int64_t Combined = I->getOperand(1).getImm() + Next->getOperand(1).getImm();
      I->getOperand(1).setImm(Combined);
      Next->eraseFromParent();
      return true;
    }
  }
  return false;
}

bool MovsucatorPeephole::optimizeBranches(MachineBasicBlock::iterator &I) {
  // Pattern 1: Compare with zero followed by branch
  // mov.cmp r1, 0
  // mov.br.nz label
  // =>
  // mov.br.nz r1, label
  if (I->getOpcode() == Movsucator::MOV_CMP) {
    auto Next = std::next(I);
    if (Next != I->getParent()->end() &&
        Next->getOpcode() == Movsucator::MOV_BR_NZ &&
        I->getOperand(1).isImm() &&
        I->getOperand(1).getImm() == 0) {
      BuildMI(*I->getParent(), I, I->getDebugLoc(),
              TII->get(Movsucator::MOV_BR_NZ_REG))
          .addReg(I->getOperand(0).getReg())
          .addMBB(Next->getOperand(0).getMBB());
      I->eraseFromParent();
      Next->eraseFromParent();
      return true;
    }
  }
  return false;
}

bool MovsucatorPeephole::mergeMoves(MachineBasicBlock::iterator &I) {
  // Pattern 1: Consecutive moves through same register
  // mov r1, r2
  // mov r3, r1
  // =>
  // mov r3, r2
  if (I->getOpcode() == Movsucator::MOV_rr) {
    auto Next = std::next(I);
    if (Next != I->getParent()->end() &&
        Next->getOpcode() == Movsucator::MOV_rr &&
        Next->getOperand(1).getReg() == I->getOperand(0).getReg()) {
      Next->getOperand(1).setReg(I->getOperand(1).getReg());
      I->eraseFromParent();
      return true;
    }
  }
  return false;
}

bool MovsucatorPeephole::eliminateDeadMoves(MachineBasicBlock::iterator &I) {
  // Eliminate moves to dead registers
  if (I->getOpcode() == Movsucator::MOV_rr) {
    Register DstReg = I->getOperand(0).getReg();
    if (Register::isVirtualRegister(DstReg) && MRI->use_empty(DstReg)) {
      I->eraseFromParent();
      return true;
    }
  }
  return false;
}

INITIALIZE_PASS(MovsucatorPeephole, DEBUG_TYPE,
                "Movsucator Peephole Optimizations", false, false)

FunctionPass *llvm::createMovsucatorPeepholePass() {
  return new MovsucatorPeephole();
}
