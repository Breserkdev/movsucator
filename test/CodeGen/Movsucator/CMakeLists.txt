set(LLVM_TARGET_DEFINITIONS Movsucator.td)

add_llvm_target_test(MovsucatorTests
  arithmetic.ll
  logical.ll
  controlflow.ll
  memory.ll
  )

add_dependencies(MovsucatorTests MovsucatorCodeGen)

set_target_properties(MovsucatorTests PROPERTIES
  FOLDER "Tests/MovsucatorTests"
  )
