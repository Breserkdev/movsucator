//===-- MovsucatorInstrLowering.cpp - Instruction Lowering Implementation -*-===//

#include "MovsucatorInstrLowering.h"
#include "MovsucatorInstrInfo.h"
#include "MovsucatorSubtarget.h"
#include "MovsucatorTargetMachine.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

MovsucatorTargetLowering::MovsucatorTargetLowering(const MovsucatorSubtarget &STI)
    : TargetLowering(STI.getTargetMachine()) {
  // Set up the register classes
  addRegisterClass(MVT::i32, &Movsucator::GPRRegClass);

  // Set operation actions
  setOperationAction(ISD::ADD, MVT::i32, Custom);
  setOperationAction(ISD::SUB, MVT::i32, Custom);
  setOperationAction(ISD::AND, MVT::i32, Custom);
  setOperationAction(ISD::OR,  MVT::i32, Custom);
  setOperationAction(ISD::XOR, MVT::i32, Custom);
  setOperationAction(ISD::BR_CC, MVT::i32, Custom);
  setOperationAction(ISD::SETCC, MVT::i32, Custom);
  
  // Operations that we expand (not custom lower)
  setOperationAction(ISD::MUL, MVT::i32, Expand);
  setOperationAction(ISD::UDIV, MVT::i32, Expand);
  setOperationAction(ISD::SDIV, MVT::i32, Expand);
  
  // Support for indirect branches
  setOperationAction(ISD::BR_JT, MVT::Other, Expand);
  setOperationAction(ISD::BRIND, MVT::Other, Expand);

  // Set calling convention and types
  setStackPointerRegisterToSaveRestore(Movsucator::S_Reg);
  computeRegisterProperties(STI.getRegisterInfo());
}

SDValue MovsucatorTargetLowering::LowerOperation(SDValue Op,
                                                SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    llvm_unreachable("Should not custom lower this!");
  case ISD::ADD:     return LowerADD(Op, DAG);
  case ISD::SUB:     return LowerSUB(Op, DAG);
  case ISD::AND:     return LowerAND(Op, DAG);
  case ISD::OR:      return LowerOR(Op, DAG);
  case ISD::XOR:     return LowerXOR(Op, DAG);
  case ISD::BR_CC:   return LowerBR_CC(Op, DAG);
  case ISD::SETCC:   return LowerSETCC(Op, DAG);
  }
}

// Lower ADD to a sequence of MOV instructions
SDValue MovsucatorTargetLowering::LowerADD(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  
  // Create a temporary register for the result
  SDValue Tmp = DAG.getRegister(Movsucator::A_Reg, MVT::i32);
  
  // MOV first operand to temp register
  SDValue MovOp0 = DAG.getNode(MovsucatorISD::MOV, DL, MVT::i32, Op0);
  
  // Use our special MOV_ADD node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_ADD, DL, MVT::i32, MovOp0, Op1);
}

// Lower SUB to a sequence of MOV instructions
SDValue MovsucatorTargetLowering::LowerSUB(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  
  // For subtraction, we'll use the fact that a - b = a + (-b)
  // First, negate the second operand using MOV operations
  SDValue Neg = DAG.getNode(MovsucatorISD::MOV_NEG, DL, MVT::i32, Op1);
  
  // Then use our ADD lowering
  return LowerADD(DAG.getNode(ISD::ADD, DL, MVT::i32, Op0, Neg), DAG);
}

// Lower AND to a sequence of MOV instructions
SDValue MovsucatorTargetLowering::LowerAND(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  
  // Use our special MOV_AND node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_AND, DL, MVT::i32, Op0, Op1);
}

// Lower OR to a sequence of MOV instructions
SDValue MovsucatorTargetLowering::LowerOR(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  
  // Use our special MOV_OR node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_OR, DL, MVT::i32, Op0, Op1);
}

// Lower XOR to a sequence of MOV instructions
SDValue MovsucatorTargetLowering::LowerXOR(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  
  // Use our special MOV_XOR node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_XOR, DL, MVT::i32, Op0, Op1);
}

// Lower branch conditions to MOV sequences
SDValue MovsucatorTargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS = Op.getOperand(2);
  SDValue RHS = Op.getOperand(3);
  SDValue Dest = Op.getOperand(4);
  
  // Create the condition using SETCC
  SDValue Cond = DAG.getNode(ISD::SETCC, DL, MVT::i32, LHS, RHS,
                            DAG.getCondCode(CC));
  
  // Use our special MOV_BR node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_BR, DL, Op.getValueType(),
                    Chain, Cond, Dest);
}

// Lower set condition code to MOV sequences
SDValue MovsucatorTargetLowering::LowerSETCC(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(2))->get();
  
  // Use our special MOV_CMP node which will be expanded later
  return DAG.getNode(MovsucatorISD::MOV_CMP, DL, Op.getValueType(),
                    LHS, RHS, DAG.getCondCode(CC));
}
