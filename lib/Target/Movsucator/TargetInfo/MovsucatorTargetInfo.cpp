//===-- MovsucatorTargetInfo.cpp - Movsucator Target Implementation -*- C++ -*-===//

#include "TargetInfo/MovsucatorTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheMovsucatorTarget() {
  static Target TheMovsucatorTarget;
  return TheMovsucatorTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMovsucatorTargetInfo() {
  RegisterTarget<Triple::movsucator> X(getTheMovsucatorTarget(), "movsucator",
                                    "Movsucator", "Movsucator");
}