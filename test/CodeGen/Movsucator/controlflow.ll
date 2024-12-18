; RUN: llc -mtriple=movsucator-unknown-unknown -verify-machineinstrs < %s | FileCheck %s

; Test control flow operations using MOV sequences

; CHECK-LABEL: branch_eq_test:
define i32 @branch_eq_test(i32 %a, i32 %b) {
entry:
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.cmp [[REG3:\$[a-z0-9]+]], [[REG1]], [[REG2]]
  ; CHECK: mov.br [[REG3]], .LBB0_1
  %cmp = icmp eq i32 %a, %b
  br i1 %cmp, label %then, label %else

then:
  ; CHECK: .LBB0_1:
  ret i32 1

else:
  ret i32 0
}

; CHECK-LABEL: branch_lt_test:
define i32 @branch_lt_test(i32 %a, i32 %b) {
entry:
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.cmp [[REG3:\$[a-z0-9]+]], [[REG1]], [[REG2]]
  ; CHECK: mov.br [[REG3]], .LBB1_1
  %cmp = icmp slt i32 %a, %b
  br i1 %cmp, label %then, label %else

then:
  ; CHECK: .LBB1_1:
  ret i32 1

else:
  ret i32 0
}

; CHECK-LABEL: select_test:
define i32 @select_test(i32 %a, i32 %b, i1 %cond) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov [[REG3:\$[a-z0-9]+]], %cond
  ; CHECK: mov.br [[REG3]], .LBB2_1
  %result = select i1 %cond, i32 %a, i32 %b
  ret i32 %result
}
