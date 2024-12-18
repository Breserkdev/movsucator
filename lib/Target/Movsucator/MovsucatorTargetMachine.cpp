#include "MovsucatorTargetMachine.h"
#include "TargetInfo/MovsucatorTargetInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

Target llvm::TheMovsucatorTarget;

namespace {
class MovsucatorTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  MovsucatorTargetObjectFile() : TargetLoweringObjectFileELF() {}

  MCSection *SelectSectionForGlobal(const GlobalObject *GO, SectionKind Kind,
                                  const TargetMachine &TM) const override {
    return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);
  }

  MCSection *getExplicitSectionGlobal(const GlobalObject *GO, SectionKind Kind,
                                    const TargetMachine &TM) const override {
    return TargetLoweringObjectFileELF::getExplicitSectionGlobal(GO, Kind, TM);
  }
};
} // end anonymous namespace

MovsucatorTargetMachine::MovsucatorTargetMachine(
    const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, "e-m:e-p:32:32-i64:64-n32-S128", TT, CPU, FS, Options,
                        RM.value_or(Reloc::Static),
                        CM.value_or(CodeModel::Small), OL),
      TLOF(std::make_unique<MovsucatorTargetObjectFile>()) {
  initAsmInfo();
}

const TargetSubtargetInfo *
MovsucatorTargetMachine::getSubtargetImpl(const Function &F) const {
  return nullptr; // We'll implement this later
}

TargetPassConfig *MovsucatorTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TargetPassConfig(*this, PM);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMovsucatorTarget() {
  RegisterTargetMachine<MovsucatorTargetMachine> X(getTheMovsucatorTarget());
}
