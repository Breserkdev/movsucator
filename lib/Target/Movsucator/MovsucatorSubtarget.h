#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORSUBTARGET_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORSUBTARGET_H

#include "MovsucatorInstrInfo.h"
#include "MovsucatorRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"

namespace llvm {

class MovsucatorSubtarget : public TargetSubtargetInfo {
  virtual void anchor();

  // Platform identification
  bool IsWindows;
  bool IsLinux;
  bool IsMacOS;

  // Subtarget features
  bool HasExtendedMov;
  bool HasMemoryBarrier;

  // CPU and feature strings
  std::string CPUString;
  std::string FSString;

  // Target instruction info
  MovsucatorInstrInfo InstrInfo;
  MovsucatorRegisterInfo RegInfo;

public:
  MovsucatorSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                      const TargetMachine &TM);

  const TargetInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const TargetRegisterInfo *getRegisterInfo() const override { return &RegInfo; }

  // Platform checks
  bool isWindows() const { return IsWindows; }
  bool isLinux() const { return IsLinux; }
  bool isMacOS() const { return IsMacOS; }

  // Feature checks
  bool hasExtendedMov() const { return HasExtendedMov; }
  bool hasMemoryBarrier() const { return HasMemoryBarrier; }

  // ABI implementations
  void setupWindowsABI(MachineFunction &MF) const;
  void setupLinuxABI(MachineFunction &MF) const;
  void setupMacOSABI(MachineFunction &MF) const;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORSUBTARGET_H
