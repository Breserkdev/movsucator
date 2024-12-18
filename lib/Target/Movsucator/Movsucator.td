//===-- Movsucator.td - Describe the Movsucator Target Machine -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

// Include the common target definitions that all targets need
include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register definitions
//===----------------------------------------------------------------------===//

include "MovsucatorRegisterInfo.td"

//===----------------------------------------------------------------------===//
// Instruction definitions
//===----------------------------------------------------------------------===//

include "MovsucatorInstrInfo.td"

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def Movsucator : Target {
  let InstructionSet = MovsucatorInstrInfo;
}