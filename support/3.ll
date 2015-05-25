; ModuleID = 'support/3.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@bbCounter = internal global i32 0
@pathctr = global [1 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x25a4880_0: %d\0A\00"

; Function Attrs: nounwind uwtable
define void @function_1(i32 %x) #0 {
; <label>:0
  %1 = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  %2 = load i32* %1, align 4
  store i32 %2, i32* %y, align 4
  %3 = load i32* %1, align 4
  store i32 %3, i32* %z, align 4
  br label %4

; <label>:4                                       ; preds = %21, %0
  %5 = load i32* %y, align 4
  %6 = icmp ugt i32 %5, 0
  br i1 %6, label %7, label %24

; <label>:7                                       ; preds = %4
  %8 = load i32* %1, align 4
  store i32 %8, i32* %z, align 4
  br label %9

; <label>:9                                       ; preds = %15, %7
  store i32 0, i32* @bbCounter
  %10 = load i32* %z, align 4
  %11 = icmp ugt i32 %10, 0
  br i1 %11, label %12, label %21

; <label>:12                                      ; preds = %9
  %13 = load i32* @bbCounter
  %14 = add i32 0, %13
  store i32 %14, i32* @bbCounter
  br label %15

; <label>:15                                      ; preds = %12
  %16 = load i32* %z, align 4
  %17 = add i32 %16, -1
  store i32 %17, i32* %z, align 4
  %18 = load i32* @bbCounter
  %19 = add i32 0, %18
  %pcptr = getelementptr [1 x i32]* @pathctr, i32 0, i32 %19
  %oldpc = load i32* %pcptr
  %20 = add i32 1, %oldpc
  store i32 %20, i32* %pcptr
  br label %9

; <label>:21                                      ; preds = %9
  %22 = load i32* %y, align 4
  %23 = add i32 %22, -1
  store i32 %23, i32* %y, align 4
  br label %4

; <label>:24                                      ; preds = %4
  %pcptr1 = getelementptr [1 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %25 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
; <label>:0
  %1 = alloca i32, align 4
  store i32 0, i32* %1
  call void @function_1(i32 10)
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
