; ModuleID = 'support/test1.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"Test Case 1\0A\00", align 1
@.str1 = private unnamed_addr constant [6 x i8] c"1:%d\0A\00", align 1
@.str2 = private unnamed_addr constant [6 x i8] c"2:%d\0A\00", align 1
@.str3 = private unnamed_addr constant [6 x i8] c"3:%d\0A\00", align 1
@bbCounter = internal global i32 0
@pathctr = global [2 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x2701860_0: %d\0A\00"
@BasicBlockPrintfFormatStr1 = private constant [22 x i8] c"Path_0x2701860_1: %d\0A\00"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
; <label>:0
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @.str, i32 0, i32 0))
  store i32 0, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 10, i32* %c, align 4
  br label %3

; <label>:3                                       ; preds = %45, %0
  store i32 0, i32* @bbCounter
  %4 = load i32* %a, align 4
  %5 = load i32* %b, align 4
  %6 = add nsw i32 %4, %5
  %7 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0), i32 %6)
  %8 = load i32* %a, align 4
  %9 = load i32* %c, align 4
  %10 = icmp sgt i32 %8, %9
  br i1 %10, label %11, label %12

; <label>:11                                      ; preds = %3
  br label %53

; <label>:12                                      ; preds = %3
  %13 = load i32* @bbCounter
  %14 = add i32 0, %13
  store i32 %14, i32* @bbCounter
  br label %15

; <label>:15                                      ; preds = %12
  %16 = load i32* %a, align 4
  %17 = srem i32 %16, 2
  %18 = icmp eq i32 %17, 0
  br i1 %18, label %19, label %25

; <label>:19                                      ; preds = %15
  %20 = load i32* @bbCounter
  %21 = add i32 0, %20
  store i32 %21, i32* @bbCounter
  br label %22

; <label>:22                                      ; preds = %19
  %23 = load i32* %a, align 4
  %24 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str2, i32 0, i32 0), i32 %23)
  br label %31

; <label>:25                                      ; preds = %15
  %26 = load i32* @bbCounter
  %27 = add i32 1, %26
  store i32 %27, i32* @bbCounter
  br label %28

; <label>:28                                      ; preds = %25
  %29 = load i32* %b, align 4
  %30 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str2, i32 0, i32 0), i32 %29)
  br label %34

; <label>:31                                      ; preds = %22
  %32 = load i32* @bbCounter
  %33 = add i32 0, %32
  store i32 %33, i32* @bbCounter
  br label %37

; <label>:34                                      ; preds = %28
  %35 = load i32* @bbCounter
  %36 = add i32 0, %35
  store i32 %36, i32* @bbCounter
  br label %37

; <label>:37                                      ; preds = %34, %31
  %38 = load i32* %a, align 4
  %39 = add nsw i32 %38, 1
  store i32 %39, i32* %a, align 4
  %40 = load i32* %b, align 4
  %41 = add nsw i32 %40, 1
  store i32 %41, i32* %b, align 4
  br label %42

; <label>:42                                      ; preds = %37
  %43 = load i32* @bbCounter
  %44 = add i32 0, %43
  store i32 %44, i32* @bbCounter
  br label %45

; <label>:45                                      ; preds = %42
  %46 = load i32* %a, align 4
  %47 = load i32* %b, align 4
  %48 = add nsw i32 %46, %47
  %49 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str3, i32 0, i32 0), i32 %48)
  %50 = load i32* @bbCounter
  %51 = add i32 0, %50
  %pcptr = getelementptr [2 x i32]* @pathctr, i32 0, i32 %51
  %oldpc = load i32* %pcptr
  %52 = add i32 1, %oldpc
  store i32 %52, i32* %pcptr
  br label %3

; <label>:53                                      ; preds = %11
  %pcptr1 = getelementptr [2 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %54 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  %pcptr3 = getelementptr [2 x i32]* @pathctr, i32 0, i32 1
  %oldpc4 = load i32* %pcptr3
  %55 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr1, i32 0, i32 0), i32 %oldpc4)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
