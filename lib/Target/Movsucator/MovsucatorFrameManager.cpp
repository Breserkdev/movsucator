#include "MovsucatorFrameManager.h"
#include "MovsucatorInstrInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

MovsucatorFrameManager::MovsucatorFrameManager(MachineFunction &mf) 
  : MF(mf) {
  // Initialize frame information
  MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Set the stack alignment - typically 16 bytes for most architectures
  MFI.setStackAlignment(16);
  
  // Initialize stack offset - grows downward
  MFI.setStackSize(0);
}

// Allocate temporary memory in the stack frame
unsigned MovsucatorFrameManager::allocateTempMem() const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Calculate required size with alignment
  const unsigned Size = 16; // Default temp allocation size
  const unsigned Align = 16; // Keep 16-byte alignment
  
  // Create a new stack object and get its offset
  int FrameIdx = MFI.CreateStackObject(Size, Align, true);
  
  // Get the offset of the allocated space
  unsigned Offset = MFI.getObjectOffset(FrameIdx);
  
  // Update stack size if necessary
  if (MFI.getStackSize() < std::abs(Offset) + Size) {
    MFI.setStackSize(std::abs(Offset) + Size);
  }
  
  return std::abs(Offset);
}

// Allocate a spill slot in the stack frame
int MovsucatorFrameManager::allocateSpillSlot(unsigned Size, unsigned Align) {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Create spill slot
  int FrameIdx = MFI.CreateSpillStackObject(Size, Align);
  
  // Update stack size if necessary
  unsigned Offset = std::abs(MFI.getObjectOffset(FrameIdx));
  if (MFI.getStackSize() < Offset + Size) {
    MFI.setStackSize(Offset + Size);
  }
  
  return FrameIdx;
}

// Clean up the stack frame
void MovsucatorFrameManager::emitFrameCleanup(MachineBasicBlock &MBB,
                                             MachineBasicBlock::iterator MBBI,
                                             const DebugLoc &DL) const {
  const MovsucatorInstrInfo *TII = 
      static_cast<const MovsucatorInstrInfo *>(MF.getSubtarget().getInstrInfo());
      
  MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t StackSize = MFI.getStackSize();
  
  if (StackSize == 0) return;
  
  // Restore stack pointer using MOV instructions
  // First, move the frame pointer to A register
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr))
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::B_Reg);  // B_Reg holds the saved frame pointer
      
  // Move it to stack pointer
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr))
      .addReg(Movsucator::S_Reg)
      .addReg(Movsucator::A_Reg);
}

// Handle variable-sized allocations
int MovsucatorFrameManager::allocateVariableSizedObject(unsigned Size, unsigned Align) {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  
  // Create a variable-sized stack object
  int FrameIdx = MFI.CreateVariableSizedObject(Align, 0);
  
  // Mark the function as having variable-sized objects
  MFI.setHasVarSizedObjects(true);
  
  return FrameIdx;
}

// Helper function to emit stack adjustment
void MovsucatorFrameManager::emitStackAdjustment(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MBBI,
                                                const DebugLoc &DL,
                                                int64_t Amount) const {
  if (Amount == 0) return;

  const MovsucatorInstrInfo *TII = 
      static_cast<const MovsucatorInstrInfo *>(MF.getSubtarget().getInstrInfo());

  // Since we can only use MOV instructions, we need to:
  // 1. MOV the current stack pointer to a temporary register
  // 2. MOV the adjustment amount to another register
  // 3. MOV the adjusted value back to stack pointer
  
  // Get the stack pointer value
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr))
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::S_Reg);
      
  // Load adjustment amount
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ri))
      .addReg(Movsucator::B_Reg)
      .addImm(Amount);
      
  // Add adjustment (using a sequence of MOVs to simulate addition)
  // This will be expanded in the instruction lowering phase
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_ADD))
      .addReg(Movsucator::A_Reg)
      .addReg(Movsucator::B_Reg);
      
  // Update stack pointer
  BuildMI(MBB, MBBI, DL, TII->get(Movsucator::MOV_rr))
      .addReg(Movsucator::S_Reg)
      .addReg(Movsucator::A_Reg);
}