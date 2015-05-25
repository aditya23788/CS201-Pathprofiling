; ModuleID = 'support/1.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@bbCounter = internal global i32 0
@pathctr = global [2 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x403bea0_0: %d\0A\00"
@BasicBlockPrintfFormatStr1 = private constant [22 x i8] c"Path_0x403bea0_1: %d\0A\00"

; Function Attrs: nounwind uwtable
define void @function_1(i32 %x) #0 {
; <label>:0
  %1 = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  br label %2

; <label>:2                                       ; preds = %31, %0
  store i32 0, i32* @bbCounter
  %3 = load i32* %1, align 4
  %4 = icmp ugt i32 %3, 0
  br i1 %4, label %6, label %5

; <label>:5                                       ; preds = %2
  br label %35

; <label>:6                                       ; preds = %2
  %7 = load i32* @bbCounter
  %8 = add i32 0, %7
  store i32 %8, i32* @bbCounter
  br label %9

; <label>:9                                       ; preds = %6
  %10 = load i32* %1, align 4
  %11 = urem i32 %10, 4
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %19

; <label>:13                                      ; preds = %9
  %14 = load i32* @bbCounter
  %15 = add i32 0, %14
  store i32 %15, i32* @bbCounter
  br label %16

; <label>:16                                      ; preds = %13
  %17 = load i32* %1, align 4
  %18 = add i32 %17, -1
  store i32 %18, i32* %1, align 4
  br label %25

; <label>:19                                      ; preds = %9
  %20 = load i32* @bbCounter
  %21 = add i32 1, %20
  store i32 %21, i32* @bbCounter
  br label %22

; <label>:22                                      ; preds = %19
  %23 = load i32* %1, align 4
  %24 = add i32 %23, -1
  store i32 %24, i32* %1, align 4
  br label %28

; <label>:25                                      ; preds = %16
  %26 = load i32* @bbCounter
  %27 = add i32 0, %26
  store i32 %27, i32* @bbCounter
  br label %31

; <label>:28                                      ; preds = %22
  %29 = load i32* @bbCounter
  %30 = add i32 0, %29
  store i32 %30, i32* @bbCounter
  br label %31

; <label>:31                                      ; preds = %28, %25
  %32 = load i32* @bbCounter
  %33 = add i32 0, %32
  %pcptr = getelementptr [2 x i32]* @pathctr, i32 0, i32 %33
  %oldpc = load i32* %pcptr
  %34 = add i32 1, %oldpc
  store i32 %34, i32* %pcptr
  br label %2

; <label>:35                                      ; preds = %5
  %pcptr1 = getelementptr [2 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %36 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  %pcptr3 = getelementptr [2 x i32]* @pathctr, i32 0, i32 1
  %oldpc4 = load i32* %pcptr3
  %37 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr1, i32 0, i32 0), i32 %oldpc4)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
; <label>:0
  %1 = alloca i32, align 4
  store i32 0, i32* %1
  call void @function_1(i32 100)
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
