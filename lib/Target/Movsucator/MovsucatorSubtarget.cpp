#include "MovsucatorSubtarget.h"
#include "MovsucatorTargetMachine.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MovsucatorGenSubtargetInfo.inc"

MovsucatorSubtarget::MovsucatorSubtarget(const Triple &TT, StringRef CPU,
                                     StringRef FS, const TargetMachine &TM)
    : TargetSubtargetInfo(TT, CPU, FS, ArrayRef<SubtargetFeatureKV>(),
                         ArrayRef<SubtargetFeatureKV>(),
                         nullptr, nullptr, nullptr, nullptr),
      IsWindows(TT.isOSWindows()),
      IsLinux(TT.isOSLinux()),
      IsMacOS(TT.isOSDarwin()),
      HasExtendedMov(false),
      HasMemoryBarrier(false),
      CPUString(CPU.str()),
      FSString(FS.str()),
      InstrInfo(),
      RegInfo() {
  // We'll parse features later when we add them
}

void MovsucatorSubtarget::anchor() {}

void MovsucatorSubtarget::setupWindowsABI(MachineFunction &MF) const {
  // We'll implement this later
}

void MovsucatorSubtarget::setupLinuxABI(MachineFunction &MF) const {
  // We'll implement this later
}

void MovsucatorSubtarget::setupMacOSABI(MachineFunction &MF) const {
  // We'll implement this later
}
