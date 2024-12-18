//===-- MovsucatorISelLowering.cpp - Movsucator DAG Lowering Implementation -*-===//

#include "MovsucatorISelLowering.h"
#include "Movsucator.h"
#include "MovsucatorSubtarget.h"
#include "MovsucatorTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "movsucator-lower"

MovsucatorTargetLowering::MovsucatorTargetLowering(const TargetMachine &TM,
                                                 const MovsucatorSubtarget &STI)
    : TargetLowering(TM) {
  // Set up the register classes
  addRegisterClass(MVT::i32, &Movsucator::GPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());
}

bool MovsucatorTargetLowering::SelectAddr(SDNode *Parent,
                                        SDValue N,
                                        SDValue &Base,
                                        SDValue &Offset) {
  SDLoc DL(N);
  EVT VT = N.getValueType();

  // Handle frame indices
  if (auto *FIN = dyn_cast<FrameIndexSDNode>(N)) {
    Base = N;
    Offset = CurDAG->getTargetConstant(0, DL, VT);
    return true;
  }

  // Handle: base + offset
  if (N.getOpcode() == ISD::ADD) {
    if (auto *CN = dyn_cast<ConstantSDNode>(N.getOperand(1))) {
      Base = N.getOperand(0);
      Offset = CurDAG->getTargetConstant(CN->getSExtValue(), DL, VT);
      return true;
    }
  }

  // Handle plain registers
  Base = N;
  Offset = CurDAG->getTargetConstant(0, DL, VT);
  return true;
}

SDValue MovsucatorTargetLowering::LowerOperation(SDValue Op,
                                               SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    llvm_unreachable("Should not custom lower this!");
  case ISD::GLOBAL_ADDRESS:
    return LowerGlobalAddress(Op, DAG);
  }
}

SDValue MovsucatorTargetLowering::LowerGlobalAddress(SDValue Op,
                                                   SelectionDAG &DAG) const {
  SDLoc DL(Op);
  EVT VT = Op.getValueType();
  GlobalAddressSDNode *GN = cast<GlobalAddressSDNode>(Op);
  const GlobalValue *GV = GN->getGlobal();
  
  // Create a constant pool entry for this global
  SDValue GA = DAG.getTargetGlobalAddress(GV, DL, VT);
  return DAG.getMoveImmediate(GA);
}