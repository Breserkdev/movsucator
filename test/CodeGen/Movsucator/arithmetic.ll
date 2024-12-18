; RUN: llc -mtriple=movsucator-unknown-unknown -verify-machineinstrs < %s | FileCheck %s

; Test basic arithmetic operations using MOV sequences

; CHECK-LABEL: add_test:
define i32 @add_test(i32 %a, i32 %b) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.add [[REG1]], [[REG2]]
  %result = add i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: sub_test:
define i32 @sub_test(i32 %a, i32 %b) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.neg [[REG2]], [[REG2]]
  ; CHECK: mov.add [[REG1]], [[REG2]]
  %result = sub i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: increment_test:
define i32 @increment_test(i32 %a) {
  ; CHECK: mov.inc [[REG1:\$[a-z0-9]+]], %a
  %result = add i32 %a, 1
  ret i32 %result
}

; CHECK-LABEL: decrement_test:
define i32 @decrement_test(i32 %a) {
  ; CHECK: mov.dec [[REG1:\$[a-z0-9]+]], %a
  %result = add i32 %a, -1
  ret i32 %result
}

; CHECK-LABEL: zero_test:
define i32 @zero_test() {
  ; CHECK: mov.zero [[REG1:\$[a-z0-9]+]]
  ret i32 0
}

; CHECK-LABEL: allones_test:
define i32 @allones_test() {
  ; CHECK: mov.allones [[REG1:\$[a-z0-9]+]]
  ret i32 -1
}
