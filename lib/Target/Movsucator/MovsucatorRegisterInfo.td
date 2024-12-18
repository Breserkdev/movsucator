//===-- MovsucatorRegisterInfo.td - Register definitions --*- tablegen -*-===//

// Register definitions
class MovsucatorReg<string n> : Register<n> {
  let Namespace = "Movsucator";
}

// Define the registers we'll use (based on the paper's 4-register requirement)
def A_Reg : MovsucatorReg<"a">;
def B_Reg : MovsucatorReg<"b">;
def C_Reg : MovsucatorReg<"c">;
def D_Reg : MovsucatorReg<"d">;

// Register classes
def GPR : RegisterClass<"Movsucator", [i32], 32, (add A_Reg, B_Reg, C_Reg, D_Reg)>;

// Calling Convention Definition
def CC_Movsucator : CallingConv<[
  CCIfType<[i8, i16], CCPromoteToType<i32>>,
  CCIfType<[i32], CCAssignToReg<[A_Reg, B_Reg, C_Reg, D_Reg]>>
]>;