; ModuleID = 'support/2.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@bbCounter = internal global i32 0
@pathctr = global [2 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x261ea40_0: %d\0A\00"
@BasicBlockPrintfFormatStr1 = private constant [22 x i8] c"Path_0x261eb20_0: %d\0A\00"

; Function Attrs: nounwind uwtable
define void @function_1(i32 %x) #0 {
; <label>:0
  %1 = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  %2 = load i32* %1, align 4
  store i32 %2, i32* %y, align 4
  br label %3

; <label>:3                                       ; preds = %10, %0
  store i32 0, i32* @bbCounter
  %4 = load i32* %1, align 4
  %5 = icmp ugt i32 %4, 0
  br i1 %5, label %7, label %6

; <label>:6                                       ; preds = %3
  br label %16

; <label>:7                                       ; preds = %3
  %8 = load i32* @bbCounter
  %9 = add i32 0, %8
  store i32 %9, i32* @bbCounter
  br label %10

; <label>:10                                      ; preds = %7
  %11 = load i32* %1, align 4
  %12 = add i32 %11, -1
  store i32 %12, i32* %1, align 4
  %13 = load i32* @bbCounter
  %14 = add i32 0, %13
  %pcptr = getelementptr [2 x i32]* @pathctr, i32 0, i32 %14
  %oldpc = load i32* %pcptr
  %15 = add i32 1, %oldpc
  store i32 %15, i32* %pcptr
  br label %3

; <label>:16                                      ; preds = %6
  %17 = load i32* %y, align 4
  store i32 %17, i32* %1, align 4
  br label %18

; <label>:18                                      ; preds = %25, %16
  store i32 0, i32* @bbCounter
  %19 = load i32* %1, align 4
  %20 = icmp ugt i32 %19, 0
  br i1 %20, label %22, label %21

; <label>:21                                      ; preds = %18
  br label %31

; <label>:22                                      ; preds = %18
  %23 = load i32* @bbCounter
  %24 = add i32 0, %23
  store i32 %24, i32* @bbCounter
  br label %25

; <label>:25                                      ; preds = %22
  %26 = load i32* %1, align 4
  %27 = add i32 %26, -1
  store i32 %27, i32* %1, align 4
  %28 = load i32* @bbCounter
  %29 = add i32 1, %28
  %pcptr1 = getelementptr [2 x i32]* @pathctr, i32 0, i32 %29
  %oldpc2 = load i32* %pcptr1
  %30 = add i32 1, %oldpc2
  store i32 %30, i32* %pcptr1
  br label %18

; <label>:31                                      ; preds = %21
  %pcptr3 = getelementptr [2 x i32]* @pathctr, i32 0, i32 0
  %oldpc4 = load i32* %pcptr3
  %32 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc4)
  %pcptr5 = getelementptr [2 x i32]* @pathctr, i32 0, i32 1
  %oldpc6 = load i32* %pcptr5
  %33 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr1, i32 0, i32 0), i32 %oldpc6)
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
