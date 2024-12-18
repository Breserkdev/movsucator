#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORREGISTERINFO_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

namespace llvm {

class MovsucatorSubtarget;

class MovsucatorRegisterInfo : public TargetRegisterInfo {
public:
  MovsucatorRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override {
    return nullptr;
  }

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                      CallingConv::ID CC) const override {
    return nullptr;
  }

  BitVector getReservedRegs(const MachineFunction &MF) const override {
    return BitVector();
  }

  bool requiresRegisterScavenging(const MachineFunction &MF) const override {
    return true;
  }
  
  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override {
    return true;
  }

  bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                          unsigned FIOperandNum,
                          RegScavenger *RS = nullptr) const override {
    return false;  // Return false to indicate no changes made
  }

  Register getFrameRegister(const MachineFunction &MF) const override {
    return 0;  // We'll update this when we implement frame handling
  }

  ArrayRef<const uint32_t *> getRegMasks() const override {
    return ArrayRef<const uint32_t *>();
  }

  ArrayRef<const char *> getRegMaskNames() const override {
    return ArrayRef<const char *>();
  }

  const RegClassWeight &getRegClassWeight(const TargetRegisterClass *RC) const override {
    static RegClassWeight RCW;
    return RCW;
  }

  unsigned getRegUnitWeight(unsigned RegUnit) const override {
    return 1;
  }

  unsigned getNumRegPressureSets() const override {
    return 0;
  }

  const char *getRegPressureSetName(unsigned Idx) const override {
    return "";
  }

  unsigned getRegPressureSetLimit(const MachineFunction &MF,
                                unsigned Idx) const override {
    return 0;
  }

  const int *getRegClassPressureSets(const TargetRegisterClass *RC) const override {
    return nullptr;
  }

  const int *getRegUnitPressureSets(unsigned RegUnit) const override {
    return nullptr;
  }

private:
  // Our four primary registers
  static const MCPhysReg S_Reg = 0;  // Special control register
  static const MCPhysReg A_Reg = 1;  // General purpose
  static const MCPhysReg B_Reg = 2;  // General purpose
  static const MCPhysReg C_Reg = 3;  // General purpose
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORREGISTERINFO_H
