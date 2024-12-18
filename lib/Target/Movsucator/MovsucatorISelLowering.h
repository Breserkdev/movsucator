//===-- MovsucatorISelLowering.h - Movsucator DAG Lowering Interface --*- C++ -*-===//

#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORISELLOWERING_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class MovsucatorSubtarget;

class MovsucatorTargetLowering : public TargetLowering {
public:
  explicit MovsucatorTargetLowering(const TargetMachine &TM,
                                  const MovsucatorSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  // Complex pattern matching methods
  static bool SelectAddr(SDNode *Parent, SDValue N, SDValue &Base, SDValue &Offset);

private:
  // Lower specific operations
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
};
} // end namespace llvm

#endif