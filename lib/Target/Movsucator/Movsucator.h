#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATOR_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATOR_H

#include "MCTargetDesc/MovsucatorMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MovsucatorTargetMachine;
class FunctionPass;

FunctionPass *createMovsucatorISelDag(MovsucatorTargetMachine &TM);
} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATOR_H
