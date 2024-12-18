add_llvm_component_library(LLVMMovsucatorInfo
  MovsucatorTargetInfo.cpp
  
  DEPENDS
  MovsucatorCommonTableGen
  
  LINK_COMPONENTS
  Support
  
  ADD_TO_COMPONENT
  Movsucator
  )