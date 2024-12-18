#include "MovsucatorMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void MovsucatorMCAsmInfo::anchor() { }

MovsucatorMCAsmInfo::MovsucatorMCAsmInfo(const Triple &TT) {
  IsLittleEndian = true;
  PointerSize = 4;
  CalleeSaveStackSlotSize = 4;
}