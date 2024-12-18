#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORTARGETMACHINE_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

namespace llvm {

class MovsucatorTargetMachine : public LLVMTargetMachine {
public:
  MovsucatorTargetMachine(const Target &T, const Triple &TT,
                          StringRef CPU, StringRef FS,
                          const TargetOptions &Options,
                          std::optional<Reloc::Model> RM = std::nullopt,
                          std::optional<CodeModel::Model> CM = std::nullopt,
                          CodeGenOpt::Level OL = CodeGenOpt::Default,
                          bool JIT = false);

  const TargetSubtargetInfo *getSubtargetImpl(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MOVSUCATORTARGETMACHINE_H
