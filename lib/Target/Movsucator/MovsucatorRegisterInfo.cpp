#include "MovsucatorRegisterInfo.h"
#include "MovsucatorSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Target/TargetFrameLowering.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "MovsucatorGenRegisterInfo.inc"

MovsucatorRegisterInfo::MovsucatorRegisterInfo()
    : TargetRegisterInfo(MovsucatorRegDesc, MovsucatorRegClasses,
                        MovsucatorRegClassID, MovsucatorRegDiffLists,
                        MovsucatorLaneMask, MovsucatorRegSubRanges,
                        MovsucatorRegSuperRegs, MovsucatorRegPressureSets) {}

bool MovsucatorRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                             int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
  // We'll implement frame index elimination later
  return false;
}
