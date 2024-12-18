//===-- MovsucatorTargetInfo.cpp - Movsucator Target Implementation -*- C++ -*-===//

#include "TargetInfo/MovsucatorTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

Target &llvm::getTheMovsucatorTarget() {
  static Target TheMovsucatorTarget;
  return TheMovsucatorTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMovsucatorTargetInfo() {
  RegisterTarget<Triple::UnknownArch> X(getTheMovsucatorTarget(), "movsucator",
                                    "Movsucator", "Movsucator");
}