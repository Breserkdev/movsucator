add_llvm_component_library(LLVMMovsucatorDesc
  MovsucatorMCTargetDesc.cpp
  MovsucatorMCAsmInfo.cpp
  MovsucatorAsmBackend.cpp
  MovsucatorMCCodeEmitter.cpp
  MovsucatorInstPrinter.cpp
  
  DEPENDS
  MovsucatorCommonTableGen
  
  LINK_COMPONENTS
  MC
  Support
  
  ADD_TO_COMPONENT
  Movsucator
  )