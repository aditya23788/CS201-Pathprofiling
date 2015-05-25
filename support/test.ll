; ModuleID = 'support/test.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@hw = global i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), align 8
@y = global i32 100, align 4
@.str1 = private unnamed_addr constant [17 x i8] c"Enter a number: \00", align 1
@.str2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str3 = private unnamed_addr constant [17 x i8] c"You entered: %d\0A\00", align 1
@.str4 = private unnamed_addr constant [10 x i8] c"%s %d %u\0A\00", align 1
@.str5 = private unnamed_addr constant [10 x i8] c"%d %s %u\0A\00", align 1
@.str6 = private unnamed_addr constant [8 x i8] c"j: %d \0A\00", align 1
@bbCounter = internal global i32 0
@pathctr = global [1 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x2a33110_0: %d\0A\00"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
; <label>:0
  %1 = alloca i32, align 4
  %x = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %1
  store i32 0, i32* %x, align 4
  %2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([17 x i8]* @.str1, i32 0, i32 0))
  %3 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.str2, i32 0, i32 0), i32* %x)
  %4 = load i32* %x, align 4
  %5 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([17 x i8]* @.str3, i32 0, i32 0), i32 %4)
  %6 = load i32* %x, align 4
  %7 = load i32* @y, align 4
  %8 = icmp ugt i32 %6, %7
  br i1 %8, label %9, label %14

; <label>:9                                       ; preds = %0
  %10 = load i8** @hw, align 8
  %11 = load i32* %x, align 4
  %12 = load i32* @y, align 4
  %13 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8]* @.str4, i32 0, i32 0), i8* %10, i32 %11, i32 %12)
  br label %19

; <label>:14                                      ; preds = %0
  %15 = load i32* %x, align 4
  %16 = load i8** @hw, align 8
  %17 = load i32* @y, align 4
  %18 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8]* @.str5, i32 0, i32 0), i32 %15, i8* %16, i32 %17)
  br label %19

; <label>:19                                      ; preds = %14, %9
  store i32 0, i32* %j, align 4
  store i32 0, i32* %i, align 4
  br label %20

; <label>:20                                      ; preds = %34, %19
  store i32 0, i32* @bbCounter
  %21 = load i32* %i, align 4
  %22 = load i32* %x, align 4
  %23 = icmp slt i32 %21, %22
  br i1 %23, label %24, label %40

; <label>:24                                      ; preds = %20
  %25 = load i32* @bbCounter
  %26 = add i32 0, %25
  store i32 %26, i32* @bbCounter
  br label %27

; <label>:27                                      ; preds = %24
  %28 = load i32* %x, align 4
  %29 = load i32* %j, align 4
  %30 = add nsw i32 %29, %28
  store i32 %30, i32* %j, align 4
  br label %31

; <label>:31                                      ; preds = %27
  %32 = load i32* @bbCounter
  %33 = add i32 0, %32
  store i32 %33, i32* @bbCounter
  br label %34

; <label>:34                                      ; preds = %31
  %35 = load i32* %i, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, i32* %i, align 4
  %37 = load i32* @bbCounter
  %38 = add i32 0, %37
  %pcptr = getelementptr [1 x i32]* @pathctr, i32 0, i32 %38
  %oldpc = load i32* %pcptr
  %39 = add i32 1, %oldpc
  store i32 %39, i32* %pcptr
  br label %20

; <label>:40                                      ; preds = %20
  %41 = load i32* %j, align 4
  %42 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str6, i32 0, i32 0), i32 %41)
  %pcptr1 = getelementptr [1 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %43 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

declare i32 @__isoc99_scanf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
