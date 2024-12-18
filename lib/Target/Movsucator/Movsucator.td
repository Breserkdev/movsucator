//===-- Movsucator.td - Describe the Movsucator Target Machine -*- tablegen -*-===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

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
// Target-Specific Information
//===----------------------------------------------------------------------===//

def MovsucatorInstrInfo : InstrInfo;

def MovsucatorAsmWriter : AsmWriter {
  let AsmWriterClassName = "InstPrinter";
}

//===----------------------------------------------------------------------===//
// Processor Features and Models
//===----------------------------------------------------------------------===//

def FeatureMov : SubtargetFeature<
    "mov",
    "HasMovInstruction",
    "true",
    "Target supports MOV instruction">;

// Scheduling definitions
def MovsucatorSchedModel : SchedMachineModel {
  let CompleteModel = 0;
  let IssueWidth = 1;  // Single instruction issue
  let LoadLatency = 1; // One cycle latency for loads
  let MispredictPenalty = 0; // No branch prediction
}

// Define functional units
def ALU : FuncUnit;

// Define instruction itineraries
def IIC_MOV : InstrItinClass;

def MovsucatorItineraries : ProcessorItineraries<[ALU], [], [
  InstrItinData<IIC_MOV, [InstrStage<1, [ALU]>]>
]>;

def : ProcessorModel<"movsucator", MovsucatorSchedModel, [FeatureMov]>;

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def Movsucator : Target {
  let InstructionSet = MovsucatorInstrInfo;
  let AssemblyWriters = [MovsucatorAsmWriter];
}