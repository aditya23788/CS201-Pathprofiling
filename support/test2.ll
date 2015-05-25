; ModuleID = 'support/test2.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"Test Case 2\0A\00", align 1
@.str1 = private unnamed_addr constant [28 x i8] c"a: %d, b: %d, c: %d, e: %d\0A\00", align 1
@.str2 = private unnamed_addr constant [6 x i8] c"1:%d\0A\00", align 1
@.str3 = private unnamed_addr constant [6 x i8] c"2:%d\0A\00", align 1
@.str4 = private unnamed_addr constant [6 x i8] c"3:%d\0A\00", align 1
@.str5 = private unnamed_addr constant [6 x i8] c"4:%d\0A\00", align 1
@bbCounter = internal global i32 0
@pathctr = global [4 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x2da1570_0: %d\0A\00"
@BasicBlockPrintfFormatStr1 = private constant [22 x i8] c"Path_0x2da1570_1: %d\0A\00"
@BasicBlockPrintfFormatStr2 = private constant [22 x i8] c"Path_0x2da1570_2: %d\0A\00"
@BasicBlockPrintfFormatStr3 = private constant [22 x i8] c"Path_0x2da1570_3: %d\0A\00"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
; <label>:0
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @.str, i32 0, i32 0))
  store i32 0, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 0, i32* %c, align 4
  store i32 20, i32* %d, align 4
  br label %3

; <label>:3                                       ; preds = %68, %0
  store i32 0, i32* @bbCounter
  %4 = load i32* %a, align 4
  %5 = load i32* %b, align 4
  %6 = load i32* %c, align 4
  %7 = load i32* %d, align 4
  %8 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([28 x i8]* @.str1, i32 0, i32 0), i32 %4, i32 %5, i32 %6, i32 %7)
  %9 = load i32* %a, align 4
  %10 = load i32* %d, align 4
  %11 = icmp sgt i32 %9, %10
  br i1 %11, label %12, label %13

; <label>:12                                      ; preds = %3
  br label %74

; <label>:13                                      ; preds = %3
  %14 = load i32* @bbCounter
  %15 = add i32 0, %14
  store i32 %15, i32* @bbCounter
  br label %16

; <label>:16                                      ; preds = %13
  %17 = load i32* %b, align 4
  %18 = srem i32 %17, 2
  %19 = icmp eq i32 %18, 0
  br i1 %19, label %20, label %26

; <label>:20                                      ; preds = %16
  %21 = load i32* @bbCounter
  %22 = add i32 0, %21
  store i32 %22, i32* @bbCounter
  br label %23

; <label>:23                                      ; preds = %20
  %24 = load i32* %b, align 4
  %25 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str2, i32 0, i32 0), i32 %24)
  br label %32

; <label>:26                                      ; preds = %16
  %27 = load i32* @bbCounter
  %28 = add i32 2, %27
  store i32 %28, i32* @bbCounter
  br label %29

; <label>:29                                      ; preds = %26
  %30 = load i32* %b, align 4
  %31 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str3, i32 0, i32 0), i32 %30)
  br label %35

; <label>:32                                      ; preds = %23
  %33 = load i32* @bbCounter
  %34 = add i32 0, %33
  store i32 %34, i32* @bbCounter
  br label %38

; <label>:35                                      ; preds = %29
  %36 = load i32* @bbCounter
  %37 = add i32 0, %36
  store i32 %37, i32* @bbCounter
  br label %38

; <label>:38                                      ; preds = %35, %32
  %39 = load i32* %b, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, i32* %b, align 4
  %41 = load i32* %c, align 4
  %42 = add nsw i32 %41, 1
  store i32 %42, i32* %c, align 4
  br label %43

; <label>:43                                      ; preds = %38
  %44 = load i32* @bbCounter
  %45 = add i32 0, %44
  store i32 %45, i32* @bbCounter
  br label %46

; <label>:46                                      ; preds = %43
  %47 = load i32* %b, align 4
  %48 = srem i32 %47, 3
  %49 = icmp eq i32 %48, 0
  br i1 %49, label %50, label %56

; <label>:50                                      ; preds = %46
  %51 = load i32* @bbCounter
  %52 = add i32 0, %51
  store i32 %52, i32* @bbCounter
  br label %53

; <label>:53                                      ; preds = %50
  %54 = load i32* %b, align 4
  %55 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str4, i32 0, i32 0), i32 %54)
  br label %62

; <label>:56                                      ; preds = %46
  %57 = load i32* @bbCounter
  %58 = add i32 1, %57
  store i32 %58, i32* @bbCounter
  br label %59

; <label>:59                                      ; preds = %56
  %60 = load i32* %b, align 4
  %61 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str5, i32 0, i32 0), i32 %60)
  br label %65

; <label>:62                                      ; preds = %53
  %63 = load i32* @bbCounter
  %64 = add i32 0, %63
  store i32 %64, i32* @bbCounter
  br label %68

; <label>:65                                      ; preds = %59
  %66 = load i32* @bbCounter
  %67 = add i32 0, %66
  store i32 %67, i32* @bbCounter
  br label %68

; <label>:68                                      ; preds = %65, %62
  %69 = load i32* %a, align 4
  %70 = add nsw i32 %69, 1
  store i32 %70, i32* %a, align 4
  %71 = load i32* @bbCounter
  %72 = add i32 0, %71
  %pcptr = getelementptr [4 x i32]* @pathctr, i32 0, i32 %72
  %oldpc = load i32* %pcptr
  %73 = add i32 1, %oldpc
  store i32 %73, i32* %pcptr
  br label %3

; <label>:74                                      ; preds = %12
  %pcptr1 = getelementptr [4 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %75 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  %pcptr3 = getelementptr [4 x i32]* @pathctr, i32 0, i32 1
  %oldpc4 = load i32* %pcptr3
  %76 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr1, i32 0, i32 0), i32 %oldpc4)
  %pcptr5 = getelementptr [4 x i32]* @pathctr, i32 0, i32 2
  %oldpc6 = load i32* %pcptr5
  %77 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr2, i32 0, i32 0), i32 %oldpc6)
  %pcptr7 = getelementptr [4 x i32]* @pathctr, i32 0, i32 3
  %oldpc8 = load i32* %pcptr7
  %78 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr3, i32 0, i32 0), i32 %oldpc8)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
