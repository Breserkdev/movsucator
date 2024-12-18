; RUN: llc -march=movsucator < %s | FileCheck %s

define i32 @test_shl(i32 %a) nounwind {
; CHECK-LABEL: test_shl:
; CHECK:       # %bb.0:
; CHECK:       mov [[REG1:r[0-9]+]], %a
; CHECK:       mov [[REG2:r[0-9]+]], [[REG1]]
; CHECK:       mov [[REG2]], [[REG2]]
; CHECK:       mov [[REG2]], [[REG2]]
; CHECK:       mov %ret, [[REG2]]
  %1 = shl i32 %a, 2
  ret i32 %1
}

define i32 @test_lshr(i32 %a) nounwind {
; CHECK-LABEL: test_lshr:
; CHECK:       # %bb.0:
; CHECK:       mov [[REG1:r[0-9]+]], %a
; CHECK:       mov [[REG2:r[0-9]+]], 0xfffffffc
; CHECK:       mov [[REG1]], [[REG1]]
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov %ret, [[REG1]]
  %1 = lshr i32 %a, 2
  ret i32 %1
}

define i32 @test_ashr(i32 %a) nounwind {
; CHECK-LABEL: test_ashr:
; CHECK:       # %bb.0:
; CHECK:       mov [[REG1:r[0-9]+]], %a
; CHECK:       mov [[REG2:r[0-9]+]], 0xfffffffc
; CHECK:       mov [[REG1]], [[REG1]]
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov %ret, [[REG1]]
  %1 = ashr i32 %a, 2
  ret i32 %1
}

define i32 @test_rotl(i32 %a) nounwind {
; CHECK-LABEL: test_rotl:
; CHECK:       # %bb.0:
; CHECK:       mov [[REG1:r[0-9]+]], %a
; CHECK:       mov [[REG2:r[0-9]+]], 0x3
; CHECK:       mov [[REG1]], [[REG1]]
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov [[REG2:r[0-9]+]], 0xfffffffc
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov %ret, [[REG1]]
  %1 = call i32 @llvm.rotl.i32(i32 %a, i32 2)
  ret i32 %1
}

define i32 @test_rotr(i32 %a) nounwind {
; CHECK-LABEL: test_rotr:
; CHECK:       # %bb.0:
; CHECK:       mov [[REG1:r[0-9]+]], %a
; CHECK:       mov [[REG2:r[0-9]+]], 0xfffffffc
; CHECK:       mov [[REG1]], [[REG1]]
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov [[REG2:r[0-9]+]], 0x3
; CHECK:       mov [[REG1]], [[REG1]], [[REG2]]
; CHECK:       mov %ret, [[REG1]]
  %1 = call i32 @llvm.rotr.i32(i32 %a, i32 2)
  ret i32 %1
}

declare i32 @llvm.rotl.i32(i32 %a, i32 %b)
declare i32 @llvm.rotr.i32(i32 %a, i32 %b)
