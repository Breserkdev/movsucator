#ifndef LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORINSTPRINTER_H
#define LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
class MCOperand;

class MovsucatorInstPrinter : public MCInstPrinter {
public:
  MovsucatorInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                      const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                const MCSubtargetInfo &STI, raw_ostream &O) override;
  void printRegName(raw_ostream &OS, MCRegister Reg) const override;
  void printOperand(const MCInst *MI, unsigned OpNo, const MCSubtargetInfo &STI,
                   raw_ostream &O);
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_MOVSUCATOR_MCTARGETDESC_MOVSUCATORINSTPRINTER_H
