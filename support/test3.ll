; ModuleID = 'support/test3.bc'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"Test Case 1\0A\00", align 1
@.str1 = private unnamed_addr constant [8 x i8] c"1a: %d\0A\00", align 1
@.str2 = private unnamed_addr constant [8 x i8] c"1b: %d\0A\00", align 1
@.str3 = private unnamed_addr constant [8 x i8] c"1c: %d\0A\00", align 1
@.str4 = private unnamed_addr constant [8 x i8] c"2a: %d\0A\00", align 1
@.str5 = private unnamed_addr constant [8 x i8] c"2c: %d\0A\00", align 1
@.str6 = private unnamed_addr constant [8 x i8] c"2b: %d\0A\00", align 1
@.str7 = private unnamed_addr constant [8 x i8] c"3b: %d\0A\00", align 1
@.str8 = private unnamed_addr constant [8 x i8] c"4bc: %d\00", align 1
@.str9 = private unnamed_addr constant [8 x i8] c"4a: %d\0A\00", align 1
@.str10 = private unnamed_addr constant [8 x i8] c"4b: %d\0A\00", align 1
@.str11 = private unnamed_addr constant [8 x i8] c"5a: %d\0A\00", align 1
@.str12 = private unnamed_addr constant [8 x i8] c"6a: %d\0A\00", align 1
@bbCounter = internal global i32 0
@pathctr = global [6 x i32] zeroinitializer, align 16
@BasicBlockPrintfFormatStr = private constant [22 x i8] c"Path_0x4231930_0: %d\0A\00"
@BasicBlockPrintfFormatStr1 = private constant [22 x i8] c"Path_0x4231930_1: %d\0A\00"
@BasicBlockPrintfFormatStr2 = private constant [22 x i8] c"Path_0x4231930_2: %d\0A\00"
@BasicBlockPrintfFormatStr3 = private constant [22 x i8] c"Path_0x4231930_3: %d\0A\00"
@BasicBlockPrintfFormatStr4 = private constant [22 x i8] c"Path_0x4231930_4: %d\0A\00"
@BasicBlockPrintfFormatStr5 = private constant [22 x i8] c"Path_0x4231930_5: %d\0A\00"

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
  store i32 0, i32* %c, align 4
  br label %3

; <label>:3                                       ; preds = %100, %0
  store i32 0, i32* @bbCounter
  %4 = load i32* %a, align 4
  %5 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str1, i32 0, i32 0), i32 %4)
  %6 = load i32* %a, align 4
  %7 = icmp sgt i32 %6, 20
  br i1 %7, label %8, label %9

; <label>:8                                       ; preds = %3
  br label %108

; <label>:9                                       ; preds = %3
  %10 = load i32* @bbCounter
  %11 = add i32 0, %10
  store i32 %11, i32* @bbCounter
  br label %12

; <label>:12                                      ; preds = %9
  %13 = load i32* %b, align 4
  %14 = srem i32 %13, 2
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %16, label %22

; <label>:16                                      ; preds = %12
  %17 = load i32* @bbCounter
  %18 = add i32 0, %17
  store i32 %18, i32* @bbCounter
  br label %19

; <label>:19                                      ; preds = %16
  %20 = load i32* %b, align 4
  %21 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str2, i32 0, i32 0), i32 %20)
  br label %28

; <label>:22                                      ; preds = %12
  %23 = load i32* @bbCounter
  %24 = add i32 4, %23
  store i32 %24, i32* @bbCounter
  br label %25

; <label>:25                                      ; preds = %22
  %26 = load i32* %c, align 4
  %27 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str3, i32 0, i32 0), i32 %26)
  br label %49

; <label>:28                                      ; preds = %19
  %29 = load i32* @bbCounter
  %30 = add i32 0, %29
  store i32 %30, i32* @bbCounter
  br label %31

; <label>:31                                      ; preds = %28
  %32 = load i32* %a, align 4
  %33 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str4, i32 0, i32 0), i32 %32)
  %34 = load i32* %c, align 4
  %35 = srem i32 %34, 3
  %36 = icmp eq i32 %35, 0
  br i1 %36, label %37, label %43

; <label>:37                                      ; preds = %31
  %38 = load i32* @bbCounter
  %39 = add i32 0, %38
  store i32 %39, i32* @bbCounter
  br label %40

; <label>:40                                      ; preds = %37
  %41 = load i32* %c, align 4
  %42 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str5, i32 0, i32 0), i32 %41)
  br label %52

; <label>:43                                      ; preds = %31
  %44 = load i32* @bbCounter
  %45 = add i32 2, %44
  store i32 %45, i32* @bbCounter
  br label %46

; <label>:46                                      ; preds = %43
  %47 = load i32* %b, align 4
  %48 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str6, i32 0, i32 0), i32 %47)
  br label %58

; <label>:49                                      ; preds = %25
  %50 = load i32* @bbCounter
  %51 = add i32 0, %50
  store i32 %51, i32* @bbCounter
  br label %55

; <label>:52                                      ; preds = %40
  %53 = load i32* @bbCounter
  %54 = add i32 0, %53
  store i32 %54, i32* @bbCounter
  br label %55

; <label>:55                                      ; preds = %52, %49
  %56 = load i32* %b, align 4
  %57 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str7, i32 0, i32 0), i32 %56)
  br label %61

; <label>:58                                      ; preds = %46
  %59 = load i32* @bbCounter
  %60 = add i32 0, %59
  store i32 %60, i32* @bbCounter
  br label %64

; <label>:61                                      ; preds = %55
  %62 = load i32* @bbCounter
  %63 = add i32 0, %62
  store i32 %63, i32* @bbCounter
  br label %64

; <label>:64                                      ; preds = %61, %58
  %65 = load i32* %b, align 4
  %66 = add nsw i32 %65, 1
  store i32 %66, i32* %b, align 4
  %67 = load i32* %c, align 4
  %68 = add nsw i32 %67, 1
  store i32 %68, i32* %c, align 4
  %69 = load i32* %b, align 4
  %70 = load i32* %c, align 4
  %71 = add nsw i32 %69, %70
  %72 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str8, i32 0, i32 0), i32 %71)
  %73 = load i32* %a, align 4
  %74 = srem i32 %73, 2
  %75 = icmp eq i32 %74, 0
  br i1 %75, label %76, label %82

; <label>:76                                      ; preds = %64
  %77 = load i32* @bbCounter
  %78 = add i32 0, %77
  store i32 %78, i32* @bbCounter
  br label %79

; <label>:79                                      ; preds = %76
  %80 = load i32* %a, align 4
  %81 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str9, i32 0, i32 0), i32 %80)
  br label %88

; <label>:82                                      ; preds = %64
  %83 = load i32* @bbCounter
  %84 = add i32 1, %83
  store i32 %84, i32* @bbCounter
  br label %85

; <label>:85                                      ; preds = %82
  %86 = load i32* %b, align 4
  %87 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str10, i32 0, i32 0), i32 %86)
  br label %94

; <label>:88                                      ; preds = %79
  %89 = load i32* @bbCounter
  %90 = add i32 0, %89
  store i32 %90, i32* @bbCounter
  br label %91

; <label>:91                                      ; preds = %88
  %92 = load i32* %a, align 4
  %93 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str11, i32 0, i32 0), i32 %92)
  br label %97

; <label>:94                                      ; preds = %85
  %95 = load i32* @bbCounter
  %96 = add i32 0, %95
  store i32 %96, i32* @bbCounter
  br label %100

; <label>:97                                      ; preds = %91
  %98 = load i32* @bbCounter
  %99 = add i32 0, %98
  store i32 %99, i32* @bbCounter
  br label %100

; <label>:100                                     ; preds = %97, %94
  %101 = load i32* %a, align 4
  %102 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str12, i32 0, i32 0), i32 %101)
  %103 = load i32* %a, align 4
  %104 = add nsw i32 %103, 1
  store i32 %104, i32* %a, align 4
  %105 = load i32* @bbCounter
  %106 = add i32 0, %105
  %pcptr = getelementptr [6 x i32]* @pathctr, i32 0, i32 %106
  %oldpc = load i32* %pcptr
  %107 = add i32 1, %oldpc
  store i32 %107, i32* %pcptr
  br label %3

; <label>:108                                     ; preds = %8
  %pcptr1 = getelementptr [6 x i32]* @pathctr, i32 0, i32 0
  %oldpc2 = load i32* %pcptr1
  %109 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr, i32 0, i32 0), i32 %oldpc2)
  %pcptr3 = getelementptr [6 x i32]* @pathctr, i32 0, i32 1
  %oldpc4 = load i32* %pcptr3
  %110 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr1, i32 0, i32 0), i32 %oldpc4)
  %pcptr5 = getelementptr [6 x i32]* @pathctr, i32 0, i32 2
  %oldpc6 = load i32* %pcptr5
  %111 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr2, i32 0, i32 0), i32 %oldpc6)
  %pcptr7 = getelementptr [6 x i32]* @pathctr, i32 0, i32 3
  %oldpc8 = load i32* %pcptr7
  %112 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr3, i32 0, i32 0), i32 %oldpc8)
  %pcptr9 = getelementptr [6 x i32]* @pathctr, i32 0, i32 4
  %oldpc10 = load i32* %pcptr9
  %113 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr4, i32 0, i32 0), i32 %oldpc10)
  %pcptr11 = getelementptr [6 x i32]* @pathctr, i32 0, i32 5
  %oldpc12 = load i32* %pcptr11
  %114 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([22 x i8]* @BasicBlockPrintfFormatStr5, i32 0, i32 0), i32 %oldpc12)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
