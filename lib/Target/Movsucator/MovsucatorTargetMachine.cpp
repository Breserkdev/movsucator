#include "MovsucatorTargetMachine.h"
#include "MovsucatorSubtarget.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>

using namespace llvm;

Target llvm::TheMovsucatorTarget;

namespace {
class MovsucatorTargetObjectFile : public TargetLoweringObjectFile {
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override {
    TargetLoweringObjectFile::Initialize(Ctx, TM);
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

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMovsucatorTarget() {
  RegisterTargetMachine<MovsucatorTargetMachine> X(TheMovsucatorTarget);
}
