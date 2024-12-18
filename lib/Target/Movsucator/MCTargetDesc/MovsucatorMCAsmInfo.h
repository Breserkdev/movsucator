#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORMCASMINFO_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class MovsucatorMCAsmInfo : public MCAsmInfoELF {
public:
  explicit MovsucatorMCAsmInfo(const Triple &TT, const MCTargetOptions &Options);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORMCASMINFO_H
