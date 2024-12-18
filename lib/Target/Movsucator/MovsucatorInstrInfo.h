#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRINFO_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRINFO_H

#include "MovsucatorRegisterInfo.h"
#include "MovsucatorInstrCodes.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

// Generated files
#include "MovsucatorGenInstrInfo.inc"

namespace llvm {

class MovsucatorInstrInfo : public TargetInstrInfo {
  const MovsucatorRegisterInfo RI;

protected:
  // Bit manipulation helpers
  void emitShiftLeftSequence(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            const DebugLoc &DL,
                            Register Rd, Register Rs, Register ShiftAmt) const;
                            
  void emitShiftRightSequence(MachineBasicBlock &MBB,
                             MachineBasicBlock::iterator MBBI,
                             const DebugLoc &DL,
                             Register Rd, Register Rs, Register ShiftAmt) const;
                             
  void emitAndSequence(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator MBBI,
                       const DebugLoc &DL,
                       Register Rd, Register Rs1, Register Rs2) const;
                       
  void emitOrSequence(MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator MBBI,
                      const DebugLoc &DL,
                      Register Rd, Register Rs1, Register Rs2) const;
                      
  void emitXorSequence(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator MBBI,
                       const DebugLoc &DL,
                       Register Rd, Register Rs1, Register Rs2) const;
                       
  void emitNotSequence(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator MBBI,
                       const DebugLoc &DL,
                       Register Rd, Register Rs) const;

  // Arithmetic helpers
  void emitAdditionSequence(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           const DebugLoc &DL,
                           Register Rd, Register Rs1, Register Rs2) const;
                           
  void emitSubtractionSequence(MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator MBBI,
                              const DebugLoc &DL,
                              Register Rd, Register Rs1, Register Rs2) const;
                              
  void emitMultiplicationSequence(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MBBI,
                                 const DebugLoc &DL,
                                 Register Rd, Register Rs1, Register Rs2) const;
                                 
  void emitConditionalBranch(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            const DebugLoc &DL,
                            Register Cond,
                            MachineBasicBlock *Target) const;
                            
  // Frame management helpers
  void emitFrameSetup(MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator MBBI,
                      const DebugLoc &DL,
                      unsigned FrameSize) const;
                      
  void emitFrameDestroy(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MBBI,
                        const DebugLoc &DL) const;

  // Helper function for temporary memory allocation
  unsigned allocateTempMem(MachineFunction &MF) const;

public:
  explicit MovsucatorInstrInfo();

  /// getRegisterInfo - TargetInstrInfo is a superset of TargetRegisterInfo.
  /// As such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  const MovsucatorRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB,
                   MachineBasicBlock::iterator MI, const DebugLoc &DL,
                   MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                          MachineBasicBlock::iterator MI,
                          Register SrcReg, bool isKill, int FrameIndex,
                          const TargetRegisterClass *RC,
                          const TargetRegisterInfo *TRI,
                          Register VReg = Register()) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI,
                           Register DestReg, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg = Register()) const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override {
    return false;
  }

  // MOV instruction classification helpers
  bool isMOVImmediate(const MachineInstr &MI) const;
  bool isMOVRegister(const MachineInstr &MI) const;
  bool isMOVMemory(const MachineInstr &MI) const;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRINFO_H
