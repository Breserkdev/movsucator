#include "MovsucatorMCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

MovsucatorMCAsmInfo::MovsucatorMCAsmInfo(const Triple &TT,
                                     const MCTargetOptions &Options) {
  CommentString = "//";
  SupportsDebugInformation = true;
}