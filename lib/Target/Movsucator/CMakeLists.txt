# First declare that this is a target
add_llvm_component_group(Movsucator)

set(LLVM_TARGET_DEFINITIONS Movsucator.td)

# Generate TableGen files
tablegen(LLVM MovsucatorGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MovsucatorGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM MovsucatorGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM MovsucatorGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM MovsucatorGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM MovsucatorGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM MovsucatorGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM MovsucatorGenCallingConv.inc -gen-callingconv)

add_public_tablegen_target(MovsucatorCommonTableGen)

# Add the target library
add_llvm_target(MovsucatorCodeGen
  MovsucatorFrameManager.cpp
  MovsucatorISelLowering.cpp
  MovsucatorInstrInfo.cpp
  MovsucatorRegisterInfo.cpp
  MovsucatorSubtarget.cpp
  MovsucatorTargetMachine.cpp
  
  DEPENDS
  MovsucatorCommonTableGen
  intrinsics_gen
  
  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  MovsucatorDesc
  MovsucatorInfo
  SelectionDAG
  Support
  Target
  GlobalISel
  )

# Add subdirectories
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)