//===-- MovsucatorMCTargetDesc.h - Movsucator Target Desc -------*- C++ -*-===//

#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORMCTARGETDESC_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class Triple;

Target &getTheMovsucatorTarget();

} // End llvm namespace

// Defines symbolic names for Movsucator registers
#define GET_REGINFO_ENUM
#include "MovsucatorGenRegisterInfo.inc"

// Defines symbolic names for Movsucator instructions
#define GET_INSTRINFO_ENUM
#include "MovsucatorGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "MovsucatorGenSubtargetInfo.inc"

#endif // MOVSUCATOR_MC_TARGET_DESC_H
