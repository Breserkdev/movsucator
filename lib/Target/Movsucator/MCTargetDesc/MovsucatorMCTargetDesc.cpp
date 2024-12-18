//===-- MovsucatorMCTargetDesc.cpp - Movsucator Target Desc -------------===//

#include "MovsucatorMCTargetDesc.h"
#include "MovsucatorInstPrinter.h"
#include "MovsucatorMCAsmInfo.h"
#include "TargetInfo/MovsucatorTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "MovsucatorGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MovsucatorGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "MovsucatorGenRegisterInfo.inc"

MCInstrInfo *createMovsucatorMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitMovsucatorMCInstrInfo(X);
  return X;
}

MCRegisterInfo *createMovsucatorMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMovsucatorMCRegisterInfo(X, 0);
  return X;
}

MCSubtargetInfo *createMovsucatorMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                              StringRef FS) {
  return createMovsucatorMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMovsucatorTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<MovsucatorMCAsmInfo> X(getTheMovsucatorTarget());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheMovsucatorTarget(),
                                    createMovsucatorMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheMovsucatorTarget(),
                                  createMovsucatorMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheMovsucatorTarget(),
                                       createMovsucatorMCSubtargetInfo);
}
