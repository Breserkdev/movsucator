#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORFRAMEMANAGER_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORFRAMEMANAGER_H

#include "llvm/CodeGen/MachineBasicBlock.h"

namespace llvm {

class MovsucatorFrameManager {
  MachineFunction &MF;

public:
  explicit MovsucatorFrameManager(MachineFunction &mf);

  // Allocate temporary memory in the stack frame
  unsigned allocateTempMem() const;

  // Helper function to emit stack adjustment
  void emitStackAdjustment(MachineBasicBlock &MBB,
                          MachineBasicBlock::iterator MBBI,
                          const DebugLoc &DL,
                          int64_t Amount) const;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORFRAMEMANAGER_H