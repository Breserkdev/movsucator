; RUN: llc -mtriple=movsucator-unknown-unknown -verify-machineinstrs < %s | FileCheck %s

; Test memory operations using MOV sequences

; CHECK-LABEL: load_test:
define i32 @load_test(i32* %ptr) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %ptr
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], [[[REG1]]]
  %val = load i32, i32* %ptr
  ret i32 %val
}

; CHECK-LABEL: store_test:
define void @store_test(i32 %val, i32* %ptr) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %val
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %ptr
  ; CHECK: mov [[[REG2]]], [[REG1]]
  store i32 %val, i32* %ptr
  ret void
}

; CHECK-LABEL: array_access_test:
define i32 @array_access_test(i32* %arr, i32 %idx) {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %arr
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], %idx
  ; CHECK: mov.add [[REG1]], [[REG2]]
  ; CHECK: mov [[REG3:\$[a-z0-9]+]], [[[REG1]]]
  %ptr = getelementptr i32, i32* %arr, i32 %idx
  %val = load i32, i32* %ptr
  ret i32 %val
}

; CHECK-LABEL: stack_alloc_test:
define i32 @stack_alloc_test() {
  ; CHECK: mov [[REG1:\$[a-z0-9]+]], %sp
  ; CHECK: mov.add [[REG1]], -4
  ; CHECK: mov %sp, [[REG1]]
  %ptr = alloca i32
  ; CHECK: mov [[REG2:\$[a-z0-9]+]], 42
  ; CHECK: mov [[[REG1]]], [[REG2]]
  store i32 42, i32* %ptr
  ; CHECK: mov [[REG3:\$[a-z0-9]+]], [[[REG1]]]
  %val = load i32, i32* %ptr
  ret i32 %val
}
