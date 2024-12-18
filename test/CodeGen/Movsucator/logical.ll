; RUN: llc -mtriple=movsucator-unknown-unknown -verify-machineinstrs < %s | FileCheck %s

; Test logical operations using MOV sequences

; CHECK-LABEL: and_test:
define i32 @and_test(i32 %a, i32 %b) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.and [[REG1]], [[REG2]]
  %result = and i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: or_test:
define i32 @or_test(i32 %a, i32 %b) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.or [[REG1]], [[REG2]]
  %result = or i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: xor_test:
define i32 @xor_test(i32 %a, i32 %b) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %a
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %b
  ; CHECK: mov.xor [[REG1]], [[REG2]]
  %result = xor i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: and_zero_test:
define i32 @and_zero_test(i32 %a) {
  ; CHECK: mov.zero [[REG1:\$[a-z0-9]+]]
  %result = and i32 %a, 0
  ret i32 %result
}

; CHECK-LABEL: or_allones_test:
define i32 @or_allones_test(i32 %a) {
  ; CHECK: mov.allones [[REG1:\$[a-z0-9]+]]
  %result = or i32 %a, -1
  ret i32 %result
}
