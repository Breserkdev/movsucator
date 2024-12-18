; RUN: llc -march=movsucator < %s | FileCheck %s

; Test basic MOV operations
define i32 @test_mov_basic(i32 %a, i32 %b) {
  ; CHECK: mov.rr {{.*}}, {{.*}}
  %1 = add i32 %a, %b
  ret i32 %1
}

; Test immediate loading
define i32 @test_mov_immediate() {
  ; CHECK: mov.ri {{.*}}, #42
  ret i32 42
}

; Test memory operations
define i32 @test_mov_memory(i32* %ptr) {
  ; CHECK: mov.mr {{.*}}, [{{.*}}]
  %1 = load i32, i32* %ptr
  ; CHECK: mov.rm [{{.*}}], {{.*}}
  store i32 %1, i32* %ptr
  ret i32 %1
}

; Test addition using MOV
define i32 @test_mov_add(i32 %a, i32 %b) {
  ; CHECK: mov.add {{.*}}, {{.*}}, {{.*}}
  %1 = add i32 %a, %b
  ret i32 %1
}

; Test subtraction using MOV
define i32 @test_mov_sub(i32 %a, i32 %b) {
  ; CHECK: mov.sub {{.*}}, {{.*}}, {{.*}}
  %1 = sub i32 %a, %b
  ret i32 %1
}

; Test multiplication using MOV
define i32 @test_mov_mul(i32 %a, i32 %b) {
  ; CHECK: mov.mul {{.*}}, {{.*}}, {{.*}}
  %1 = mul i32 %a, %b
  ret i32 %1
}

; Test conditional branch
define void @test_mov_branch(i32 %cond) {
entry:
  ; CHECK: mov.cmpeq {{.*}}, {{.*}}
  ; CHECK: mov.je {{.*}}
  %cmp = icmp eq i32 %cond, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:
  ret void

if.end:
  ret void
}

; Test function call and return
define i32 @test_mov_call() {
  ; CHECK: mov.call {{.*}}
  %1 = call i32 @test_mov_basic(i32 1, i32 2)
  ; CHECK: mov.ret {{.*}}
  ret i32 %1
}

; Test frame setup and destroy
define void @test_mov_frame() {
  ; CHECK: mov.frame.setup
  %1 = alloca i32
  store i32 42, i32* %1
  ; CHECK: mov.frame.destroy
  ret void
}
