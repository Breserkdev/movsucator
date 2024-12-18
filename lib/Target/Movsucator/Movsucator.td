//===-- Movsucator.td - Main target description --*- tablegen -*-===//

// Include the common target definitions
include "llvm/Target/Target.td"

// Include our instruction definitions
include "MovsucatorInstrInfo.td"

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

// Define register class name
let Namespace = "Movsucator" in {
  // Basic register class
  class MovsucatorReg<string n> : Register<n> {
    let HWEncoding{3-0} = 0;
  }
}

// Define the four registers as per the paper
def A : MovsucatorReg<"a">;
def B : MovsucatorReg<"b">;
def C : MovsucatorReg<"c">;
def S : MovsucatorReg<"s">; // Changed D to S to match paper

// Register class for general purpose registers
def GPR : RegisterClass<"Movsucator", [i32], 32, (add A, B, C, S)>;

//===----------------------------------------------------------------------===//
// Target-Specific Information
//===----------------------------------------------------------------------===//

def MovsucatorInstrInfo : InstrInfo;

def MovsucatorAsmWriter : AsmWriter {
  let AsmWriterClassName = "InstPrinter";
}

//===----------------------------------------------------------------------===//
// Calling Convention Definition
//===----------------------------------------------------------------------===//

def CC_Movsucator : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[A, B, C, S]>>
]>;

//===----------------------------------------------------------------------===//
// Processor Features and Models
//===----------------------------------------------------------------------===//

def FeatureMov : SubtargetFeature<
    "mov",
    "HasMovInstruction",
    "true",
    "Target supports MOV instruction">;

def MovsucatorModel : SchedMachineModel {
  let CompleteModel = 0;
}

def : ProcessorModel<"movsucator", MovsucatorModel, [FeatureMov]>;

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def Movsucator : Target {
  let InstructionSet = MovsucatorInstrInfo;
  let AssemblyWriters = [MovsucatorAsmWriter];
}