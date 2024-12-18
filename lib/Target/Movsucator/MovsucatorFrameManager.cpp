#include "MovsucatorFrameManager.h"
#include "MovsucatorInstrInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

MovsucatorFrameManager::MovsucatorFrameManager(MachineFunction &mf) 
  : MF(mf) {
}

// Allocate temporary memory in the stack frame
unsigned MovsucatorFrameManager::allocateTempMem() const {
  // For now, just return a fixed offset
  // In a real implementation, you'd track stack allocations
  return 16;  // Return a 16-byte aligned temporary space
}

// Helper function to emit stack adjustment
void MovsucatorFrameManager::emitStackAdjustment(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MBBI,
                                                const DebugLoc &DL,
                                                int64_t Amount) const {
  if (Amount == 0) return;

  const MovsucatorInstrInfo *TII = 
      static_cast<const MovsucatorInstrInfo *>(MF.getSubtarget().getInstrInfo());

  // Emit the MOV instruction to adjust the stack
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ri))
      .addReg(Movsucator::A_Reg)
      .addImm(Amount);
}