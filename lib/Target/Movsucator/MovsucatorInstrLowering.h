//===-- MovsucatorInstrLowering.h - Instruction Lowering Interface --*- C++ -*-===//

#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRLOWERING_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRLOWERING_H

#include "Movsucator.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class MovsucatorSubtarget;

class MovsucatorTargetLowering : public TargetLowering {
public:
  explicit MovsucatorTargetLowering(const MovsucatorSubtarget &STI);

private:
  // Lower custom operations
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const;

  // Lower specific operations to MOV sequences
  SDValue LowerADD(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSUB(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerAND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerOR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerXOR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  // Custom lowering hooks
  bool shouldConvertConstantLoadToIntImm(const APInt &Imm,
                                       Type *Ty) const override {
    return true;
  }
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRLOWERING_H
