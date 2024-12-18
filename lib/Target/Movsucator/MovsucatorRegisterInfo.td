//===-- MovsucatorRegisterInfo.td - Register definitions --*- tablegen -*-===//

include "llvm/CodeGen/ValueTypes.td"
include "llvm/IR/CallingConv.td"
include "llvm/Target/Target.td"
include "llvm/Target/GlobalISel/RegisterBank.td"

// Register definitions
class MovsucatorReg<string n> : Register<n> {
  let Namespace = "Movsucator";
  let HWEncoding{3-0} = 0;
}

// Define the registers we'll use (based on the paper's 4-register requirement)
def R0 : MovsucatorReg<"r0">;
def R1 : MovsucatorReg<"r1">;
def R2 : MovsucatorReg<"r2">;
def R3 : MovsucatorReg<"r3">;

// Register classes
def GPR : RegisterClass<"Movsucator", [i32], 32, (add R0, R1, R2, R3)>;

// Calling Convention Definition
def CC_Movsucator : CallingConv<[
  CCIfType<[i8, i16], CCPromoteToType<i32>>,
  CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3]>>
]>;