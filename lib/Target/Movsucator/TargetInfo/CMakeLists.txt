include_directories(${CMAKE_CURRENT_BINARY_DIR}/..
                  ${CMAKE_CURRENT_SOURCE_DIR}/..
                  ${CMAKE_SOURCE_DIR}/llvm-project/llvm/include
                  ${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm
                  ${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/MC
                  ${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/Support
                  ${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/TargetParser
                  ${CMAKE_BINARY_DIR}/include)

add_llvm_component_library(LLVMMovsucatorInfo
  MovsucatorTargetInfo.cpp
  
  DEPENDS
  MovsucatorCommonTableGen
  
  LINK_COMPONENTS
  Support
  
  ADD_TO_COMPONENT
  Movsucator
  )