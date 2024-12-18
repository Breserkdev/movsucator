#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRCODES_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRCODES_H

namespace llvm {
namespace Movsucator {
  enum {
    MOV_rr = 0,  // Register-to-Register MOV
    MOVi,        // Immediate-to-Register MOV
    MOV_load,    // Memory Load
    MOV_store,   // Memory Store
  };
} // end namespace Movsucator
} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORINSTRCODES_H
