//===-- Movsucator.td - Describe the Movsucator Target Machine -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

// Include the common target definitions that all targets need
include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Target Description
//===----------------------------------------------------------------------===//

// Include our target-specific files
include "MovsucatorBase.td"
include "MovsucatorRegisterInfo.td"
include "MovsucatorInstrFormats.td"

//===----------------------------------------------------------------------===//
// Target Configuration
//===----------------------------------------------------------------------===//

def MovsucatorInstrInfo : InstrInfo {
  let guessInstructionProperties = 0;
}

def MovsucatorAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
}

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def Movsucator : Target {
  let InstructionSet = MovsucatorInstrInfo;
}