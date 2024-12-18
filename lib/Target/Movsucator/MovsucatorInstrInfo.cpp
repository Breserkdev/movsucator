#include "MovsucatorInstrInfo.h"
#include "MovsucatorSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

// Include instruction descriptions
#include "MovsucatorGenInstrInfo.inc"

// Include instruction implementations
#define GET_INSTRINFO_CTOR_DTOR
#include "MovsucatorGenInstrInfo.inc"

#define DEBUG_TYPE "movsucator-instrinfo"

MovsucatorInstrInfo::MovsucatorInstrInfo()
    : TargetInstrInfo(MovsucatorInsts, std::size(MovsucatorInsts)) {}

void MovsucatorInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   const DebugLoc &DL, MCRegister DestReg,
                                   MCRegister SrcReg, bool KillSrc) const {
  // Create a MOV instruction to copy between registers
  BuildMI(MBB, MI, DL, get(Movsucator::MOV_rr))
      .addReg(DestReg, RegState::Define)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

void MovsucatorInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  // Create a MOV instruction to store register to stack
  MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Get the actual stack offset
  int64_t Offset = MFI.getObjectOffset(FrameIndex);
  
  // Create a memory operand for the stack slot
  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FrameIndex),
      MachineMemOperand::MOStore, MFI.getObjectSize(FrameIndex),
      MFI.getObjectAlign(FrameIndex));

  // Emit MOV from register to memory
  BuildMI(MBB, MI, DebugLoc(), get(Movsucator::MOV_rm))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FrameIndex)
      .addImm(Offset)
      .addMemOperand(MMO);
}

void MovsucatorInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register DestReg,
    int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  // Create a MOV instruction to load register from stack
  MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Get the actual stack offset
  int64_t Offset = MFI.getObjectOffset(FrameIndex);
  
  // Create a memory operand for the stack slot
  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FrameIndex),
      MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIndex),
      MFI.getObjectAlign(FrameIndex));

  // Emit MOV from memory to register
  BuildMI(MBB, MI, DebugLoc(), get(Movsucator::MOV_mr))
      .addReg(DestReg, RegState::Define)
      .addFrameIndex(FrameIndex)
      .addImm(Offset)
      .addMemOperand(MMO);
}

unsigned MovsucatorInstrInfo::allocateTempMem(MachineFunction &MF) const {
  auto &MFI = MF.getFrameInfo();
  return MFI.CreateStackObject(8, Align(8), false);
}

void MovsucatorInstrInfo::emitAdditionSequence(MachineBasicBlock &MBB,
                                              MachineBasicBlock::iterator MBBI,
                                              const DebugLoc &DL,
                                              Register Rd, Register Rs1, Register Rs2) const {
  // Addition using MOV-only sequence:
  // 1. Save Rs1 to temp memory
  // 2. Move Rs2 to Rd
  // 3. For each bit in Rs1:
  //    - If bit is 1, shift Rd left by that position
  //    - Add the shifted values together using XOR and AND operations
  
  // Allocate temporary memory for addition
  MachineFunction *MF = MBB.getParent();
  unsigned TempSlot1 = allocateTempMem(*MF);  // For Rs1
  unsigned TempSlot2 = allocateTempMem(*MF);  // For intermediate results
  
  // Store Rs1 to temp memory
  storeRegToStackSlot(MBB, MBBI, Rs1, false, TempSlot1, nullptr, nullptr);
  
  // Move Rs2 to Rd
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_rr))
      .addReg(Rd, RegState::Define)
      .addReg(Rs2);
      
  // Create labels for our loops
  MachineBasicBlock *LoopMBB = MF->CreateMachineBasicBlock();
  MachineBasicBlock *DoneMBB = MF->CreateMachineBasicBlock();
  
  // Add the new basic blocks and maintain CFG
  MF->push_back(LoopMBB);
  MF->push_back(DoneMBB);
  
  // Add successors
  MBB.addSuccessor(LoopMBB);
  LoopMBB->addSuccessor(LoopMBB);
  LoopMBB->addSuccessor(DoneMBB);
  
  // Initialize counter
  Register CounterReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(CounterReg, RegState::Define)
      .addImm(0);
      
  // Load Rs1 from temp memory to a working register
  Register WorkReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  loadRegFromStackSlot(MBB, MBBI, WorkReg, TempSlot1, nullptr, nullptr);
  
  // Loop:
  // 1. Test bit at counter position
  // 2. If set, add shifted value
  // 3. Increment counter
  // 4. Continue until done
  
  // Emit the loop body
  auto LoopI = LoopMBB->end();
  
  // Test bit at counter position
  Register BitTestReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(*LoopMBB, LoopI, DL, get(Movsucator::MOV_rr))
      .addReg(BitTestReg, RegState::Define)
      .addReg(WorkReg);
      
  // Shift right by counter
  // Note: This will be implemented using MOV sequences in a helper function
  emitShiftRightSequence(*LoopMBB, LoopI, DL, BitTestReg, BitTestReg, CounterReg);
  
  // Test least significant bit
  Register BitMaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(*LoopMBB, LoopI, DL, get(Movsucator::MOV_ri))
      .addReg(BitMaskReg, RegState::Define)
      .addImm(1);
      
  // AND with mask
  Register TestResultReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  emitAndSequence(*LoopMBB, LoopI, DL, TestResultReg, BitTestReg, BitMaskReg);
  
  // If bit is set, add shifted value
  Register ShiftedReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(*LoopMBB, LoopI, DL, get(Movsucator::MOV_rr))
      .addReg(ShiftedReg, RegState::Define)
      .addReg(Rs2);
      
  // Shift left by counter
  emitShiftLeftSequence(*LoopMBB, LoopI, DL, ShiftedReg, ShiftedReg, CounterReg);
  
  // Add to result using XOR for addition without carry
  emitXorSequence(*LoopMBB, LoopI, DL, Rd, Rd, ShiftedReg);
  
  // Increment counter
  Register OneReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(*LoopMBB, LoopI, DL, get(Movsucator::MOV_ri))
      .addReg(OneReg, RegState::Define)
      .addImm(1);
      
  emitAdditionSequence(*LoopMBB, LoopI, DL, CounterReg, CounterReg, OneReg);
  
  // Compare counter with 32 (word size)
  Register ThirtyTwoReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
  BuildMI(*LoopMBB, LoopI, DL, get(Movsucator::MOV_ri))
      .addReg(ThirtyTwoReg, RegState::Define)
      .addImm(32);
      
  // Branch if not equal
  emitConditionalBranch(*LoopMBB, LoopI, DL, CounterReg, LoopMBB);
  
  // Fall through to done
  MBB.splice(MBBI, DoneMBB);
}

void MovsucatorInstrInfo::emitSubtractionSequence(MachineBasicBlock &MBB,
                                                 MachineBasicBlock::iterator MBBI,
                                                 const DebugLoc &DL,
                                                 Register Rd, Register Rs1, Register Rs2) const {
  // Implement subtraction using MOV sequences later
}

void MovsucatorInstrInfo::emitMultiplicationSequence(MachineBasicBlock &MBB,
                                                    MachineBasicBlock::iterator MBBI,
                                                    const DebugLoc &DL,
                                                    Register Rd, Register Rs1, Register Rs2) const {
  // Implement multiplication using MOV sequences later
}

void MovsucatorInstrInfo::emitConditionalBranch(MachineBasicBlock &MBB,
                                               MachineBasicBlock::iterator MBBI,
                                               const DebugLoc &DL,
                                               Register Cond,
                                               MachineBasicBlock *Target) const {
  // Implement conditional branches using MOV sequences later
}

void MovsucatorInstrInfo::emitFrameSetup(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MBBI,
                                        const DebugLoc &DL,
                                        unsigned FrameSize) const {
  // Implement frame setup using MOV sequences later
}

void MovsucatorInstrInfo::emitFrameDestroy(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI,
                                          const DebugLoc &DL) const {
  // Implement frame teardown using MOV sequences later
}

bool MovsucatorInstrInfo::isMOVImmediate(const MachineInstr &MI) const {
  return MI.getOpcode() == Movsucator::MOV_ri;
}

bool MovsucatorInstrInfo::isMOVRegister(const MachineInstr &MI) const {
  return MI.getOpcode() == Movsucator::MOV_rr;
}

bool MovsucatorInstrInfo::isMOVMemory(const MachineInstr &MI) const {
  return MI.getOpcode() == Movsucator::MOV_mr ||
         MI.getOpcode() == Movsucator::MOV_rm;
}

void MovsucatorInstrInfo::emitShiftLeftSequence(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MBBI,
                                                const DebugLoc &DL,
                                                Register Rd, Register Rs, Register ShiftAmt) const {
  // Shift left using MOV-only sequence:
  // For each bit position i:
  // - Test if shift amount equals i
  // - If equal, MOV Rs << i to Rd
  
  MachineFunction *MF = MBB.getParent();
  
  // Create basic blocks for the shift sequence
  MachineBasicBlock *ShiftMBB = MF->CreateMachineBasicBlock();
  MachineBasicBlock *DoneMBB = MF->CreateMachineBasicBlock();
  
  // Add the new basic blocks and maintain CFG
  MF->push_back(ShiftMBB);
  MF->push_back(DoneMBB);
  
  // Add successors
  MBB.addSuccessor(ShiftMBB);
  ShiftMBB->addSuccessor(DoneMBB);
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each possible shift amount (0 to 31)
  for (unsigned i = 0; i < 32; ++i) {
    Register ShiftValReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_ri))
        .addReg(ShiftValReg, RegState::Define)
        .addImm(i);
        
    // Compare with shift amount
    Register EqualReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitXorSequence(*ShiftMBB, ShiftMBB->end(), DL, EqualReg, ShiftValReg, ShiftAmt);
    emitNotSequence(*ShiftMBB, ShiftMBB->end(), DL, EqualReg, EqualReg);
    
    // If equal, set result to Rs << i
    if (i == 0) {
      // No shift needed
      BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_rr))
          .addReg(Rd, RegState::Define)
          .addReg(Rs);
    } else {
      // Create mask for shifted value
      Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_ri))
          .addReg(MaskReg, RegState::Define)
          .addImm((1 << (32 - i)) - 1);
          
      // AND with mask
      Register ShiftedReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      emitAndSequence(*ShiftMBB, ShiftMBB->end(), DL, ShiftedReg, Rs, MaskReg);
      
      // Update result if shift amount matches
      Register UpdatedReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      emitAndSequence(*ShiftMBB, ShiftMBB->end(), DL, UpdatedReg, ShiftedReg, EqualReg);
      emitOrSequence(*ShiftMBB, ShiftMBB->end(), DL, Rd, Rd, UpdatedReg);
    }
  }
  
  // Fall through to done
  MBB.splice(MBBI, DoneMBB);
}

void MovsucatorInstrInfo::emitShiftRightSequence(MachineBasicBlock &MBB,
                                                 MachineBasicBlock::iterator MBBI,
                                                 const DebugLoc &DL,
                                                 Register Rd, Register Rs, Register ShiftAmt) const {
  // Similar to shift left, but shift right
  // Implementation follows the same pattern as emitShiftLeftSequence
  // but with right shift masks
  
  MachineFunction *MF = MBB.getParent();
  
  // Create basic blocks for the shift sequence
  MachineBasicBlock *ShiftMBB = MF->CreateMachineBasicBlock();
  MachineBasicBlock *DoneMBB = MF->CreateMachineBasicBlock();
  
  // Add the new basic blocks and maintain CFG
  MF->push_back(ShiftMBB);
  MF->push_back(DoneMBB);
  
  // Add successors
  MBB.addSuccessor(ShiftMBB);
  ShiftMBB->addSuccessor(DoneMBB);
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each possible shift amount (0 to 31)
  for (unsigned i = 0; i < 32; ++i) {
    Register ShiftValReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_ri))
        .addReg(ShiftValReg, RegState::Define)
        .addImm(i);
        
    // Compare with shift amount
    Register EqualReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitXorSequence(*ShiftMBB, ShiftMBB->end(), DL, EqualReg, ShiftValReg, ShiftAmt);
    emitNotSequence(*ShiftMBB, ShiftMBB->end(), DL, EqualReg, EqualReg);
    
    // If equal, set result to Rs >> i
    if (i == 0) {
      // No shift needed
      BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_rr))
          .addReg(Rd, RegState::Define)
          .addReg(Rs);
    } else {
      // Create mask for shifted value
      Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      BuildMI(*ShiftMBB, ShiftMBB->end(), DL, get(Movsucator::MOV_ri))
          .addReg(MaskReg, RegState::Define)
          .addImm(~((1 << i) - 1));
          
      // AND with mask
      Register ShiftedReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      emitAndSequence(*ShiftMBB, ShiftMBB->end(), DL, ShiftedReg, Rs, MaskReg);
      
      // Update result if shift amount matches
      Register UpdatedReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
      emitAndSequence(*ShiftMBB, ShiftMBB->end(), DL, UpdatedReg, ShiftedReg, EqualReg);
      emitOrSequence(*ShiftMBB, ShiftMBB->end(), DL, Rd, Rd, UpdatedReg);
    }
  }
  
  // Fall through to done
  MBB.splice(MBBI, DoneMBB);
}

void MovsucatorInstrInfo::emitAndSequence(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI,
                                          const DebugLoc &DL,
                                          Register Rd, Register Rs1, Register Rs2) const {
  // AND using MOV-only sequence:
  // For each bit position:
  // 1. Test bit in Rs1
  // 2. Test bit in Rs2
  // 3. If both 1, set bit in result
  
  MachineFunction *MF = MBB.getParent();
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each bit position
  for (unsigned i = 0; i < 32; ++i) {
    // Create mask for current bit
    Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
        .addReg(MaskReg, RegState::Define)
        .addImm(1 << i);
        
    // Test bit in Rs1
    Register Bit1Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit1Reg, Rs1, MaskReg);
    
    // Test bit in Rs2
    Register Bit2Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit2Reg, Rs2, MaskReg);
    
    // If both bits are set, set bit in result
    Register BothSetReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, BothSetReg, Bit1Reg, Bit2Reg);
    emitOrSequence(MBB, MBBI, DL, Rd, Rd, BothSetReg);
  }
}

void MovsucatorInstrInfo::emitOrSequence(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator MBBI,
                                         const DebugLoc &DL,
                                         Register Rd, Register Rs1, Register Rs2) const {
  // OR using MOV-only sequence:
  // For each bit position:
  // 1. Test bit in Rs1
  // 2. Test bit in Rs2
  // 3. If either 1, set bit in result
  
  MachineFunction *MF = MBB.getParent();
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each bit position
  for (unsigned i = 0; i < 32; ++i) {
    // Create mask for current bit
    Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
        .addReg(MaskReg, RegState::Define)
        .addImm(1 << i);
        
    // Test bit in Rs1
    Register Bit1Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit1Reg, Rs1, MaskReg);
    
    // Test bit in Rs2
    Register Bit2Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit2Reg, Rs2, MaskReg);
    
    // If either bit is set, set bit in result
    Register EitherSetReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitOrSequence(MBB, MBBI, DL, EitherSetReg, Bit1Reg, Bit2Reg);
    emitOrSequence(MBB, MBBI, DL, Rd, Rd, EitherSetReg);
  }
}

void MovsucatorInstrInfo::emitXorSequence(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI,
                                          const DebugLoc &DL,
                                          Register Rd, Register Rs1, Register Rs2) const {
  // XOR using MOV-only sequence:
  // For each bit position:
  // 1. Test bit in Rs1
  // 2. Test bit in Rs2
  // 3. If bits differ, set bit in result
  
  MachineFunction *MF = MBB.getParent();
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each bit position
  for (unsigned i = 0; i < 32; ++i) {
    // Create mask for current bit
    Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
        .addReg(MaskReg, RegState::Define)
        .addImm(1 << i);
        
    // Test bit in Rs1
    Register Bit1Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit1Reg, Rs1, MaskReg);
    
    // Test bit in Rs2
    Register Bit2Reg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, Bit2Reg, Rs2, MaskReg);
    
    // If bits differ, set bit in result
    Register DifferReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitXorSequence(MBB, MBBI, DL, DifferReg, Bit1Reg, Bit2Reg);
    emitOrSequence(MBB, MBBI, DL, Rd, Rd, DifferReg);
  }
}

void MovsucatorInstrInfo::emitNotSequence(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MBBI,
                                          const DebugLoc &DL,
                                          Register Rd, Register Rs) const {
  // NOT using MOV-only sequence:
  // For each bit position:
  // 1. Test bit in Rs
  // 2. If 0, set bit in result
  
  MachineFunction *MF = MBB.getParent();
  
  // Initialize result to 0
  BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
      .addReg(Rd, RegState::Define)
      .addImm(0);
      
  // For each bit position
  for (unsigned i = 0; i < 32; ++i) {
    // Create mask for current bit
    Register MaskReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    BuildMI(MBB, MBBI, DL, get(Movsucator::MOV_ri))
        .addReg(MaskReg, RegState::Define)
        .addImm(1 << i);
        
    // Test bit in Rs
    Register BitReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitAndSequence(MBB, MBBI, DL, BitReg, Rs, MaskReg);
    
    // If bit is 0, set bit in result
    Register NotBitReg = MRI->createVirtualRegister(&Movsucator::GPRRegClass);
    emitXorSequence(MBB, MBBI, DL, NotBitReg, BitReg, MaskReg);
    emitOrSequence(MBB, MBBI, DL, Rd, Rd, NotBitReg);
  }
}
