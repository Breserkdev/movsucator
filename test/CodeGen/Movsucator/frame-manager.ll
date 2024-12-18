; RUN: llc -march=movsucator -verify-machineinstrs < %s | FileCheck %s

; Test frame setup and temporary memory allocation
define i32 @test_frame_setup(i32 %a, i32 %b) nounwind {
entry:
  ; CHECK: mov_push
  %temp = alloca i32
  store i32 %a, i32* %temp
  %val = load i32, i32* %temp
  ; CHECK: mov_pop
  ret i32 %val
}

; Test multiple temporary allocations
define i32 @test_multiple_temps(i32 %a, i32 %b) nounwind {
entry:
  ; CHECK: mov_push
  %temp1 = alloca i32
  %temp2 = alloca i32
  store i32 %a, i32* %temp1
  store i32 %b, i32* %temp2
  %val1 = load i32, i32* %temp1
  %val2 = load i32, i32* %temp2
  %sum = add i32 %val1, %val2
  ; CHECK: mov_pop
  ret i32 %sum
}

; Test frame cleanup
define void @test_frame_cleanup() nounwind {
entry:
  ; CHECK: mov_push
  %temp = alloca i32
  store i32 42, i32* %temp
  ; CHECK: mov_pop
  ret void
}
