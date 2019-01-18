; ModuleID = 'stm2ir'
 declare i32 @printf(i8*, ...)
 @print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
	%var1 = alloca i32
	%1= sub i32 45, 78 
	%2= sdiv i32 15, 3 
	%3= add i32 25, %2 
	%4= mul i32 %1, %3 
	store i32 %4, i32* %var1
	%5 = load i32* %var1
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %5 )
	%var2 = alloca i32
	%7= mul i32 78, 89 
	%8= sub i32 45, 482 
	%9= sub i32 %7, %8 
	%10= mul i32 63, 2 
	%11= add i32 %9, %10 
	%12= sdiv i32 11115, 4587 
	%13= add i32 %11, %12 
	store i32 %13, i32* %var2
	%14 = load i32* %var2
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %14 )
	%16 = load i32* %var1
	%17 = load i32* %var2
	%18= sub i32 %17, %16 
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %18 )
	%var3 = alloca i32
	%20 = load i32* %var2
	%21 = load i32* %var1
	%22= sub i32 %21, %20 
	%23= sdiv i32 47896, 12 
	%24= sdiv i32 1000000, 78569 
	%25= sub i32 %23, %24 
	%26= add i32 %22, %25 
	store i32 %26, i32* %var3
	%27 = load i32* %var2
	%28 = load i32* %var1
	%29= sub i32 %28, %27 
	%30 = load i32* %var3
	%31= sub i32 %30, 100 
	%32= add i32 %29, %31 
	%33 = load i32* %var3
	%34= add i32 %33, %32 
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %34 )
	%var4 = alloca i32
	%36= mul i32 75, 78 
	%37= mul i32 265, 0 
	%38= sub i32 145, %37 
	%39= sub i32 5, %38 
	%40= mul i32 %36, %39 
	store i32 %40, i32* %var4
	%41 = load i32* %var4
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %41 )
	%43 = load i32* %var4
	%44 = load i32* %var3
	%45= sub i32 %44, %43 
	%46 = load i32* %var2
	%47 = load i32* %var1
	%48= sub i32 %47, %46 
	%49= add i32 %45, %48 
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %49 )
	%51 = load i32* %var3
	%52 = load i32* %var4
	%53= sub i32 %52, %51 
	%54 = load i32* %var1
	%55= sub i32 %54, 1000 
	%56 = load i32* %var2
	%57= sub i32 %56, 100 
	%58= mul i32 %55, %57 
	%59= add i32 %53, %58 
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %59 )
	%61= sdiv i32 777, 123 
	%62 = load i32* %var4
	%63= mul i32 %62, %61 
	%64= sdiv i32 4587, 2369 
	%65= mul i32 %64, 100 
	%66= add i32 %63, %65 
	call i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %66 )
	ret i32 0
}
