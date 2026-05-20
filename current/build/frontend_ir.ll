define i32 @myFunction() {
    %1 = alloca i32, align 4

    store i32 5, i32* %1, align 4

    %2 = alloca i32, align 4

    store i32 70, i32* %2, align 4

    %3 = alloca i32, align 4

    %4 = alloca i32, align 4

    %5 = alloca i32, align 4

    store i32 200, i32* %5, align 4

    store i32 10, i32* %3, align 4

    %6 = alloca i32, align 4
    store i32 1, i32* %6, align 4

    %7 = getelementptr i32, i32* %1, i32 0

    ; loading values for binary operation

    %8 = load i32, i32* %6, align 4

    %9 = load i32, i32* %7, align 4

    %10 = add i32 %8, %9
    %11 = alloca i32, align 4
    store i32 %10, i32* %11, align 4 ; storing the answer

    store i32 %10, i32* %1, align 4

    %12 = alloca i32, align 4
    store i32 1, i32* %12, align 4

    %13 = getelementptr i32, i32* %2, i32 0

    ; loading values for binary operation

    %14 = load i32, i32* %12, align 4

    %15 = load i32, i32* %13, align 4

    %16 = add i32 %14, %15
    %17 = alloca i32, align 4
    store i32 %16, i32* %17, align 4 ; storing the answer

    store i32 %16, i32* %2, align 4

    store i32 100, i32* %4, align 4

    %18 = alloca i32, align 4
    store i32 1, i32* %18, align 4

    %19 = getelementptr i32, i32* %1, i32 0

    ; loading values for binary operation

    %20 = load i32, i32* %18, align 4

    %21 = load i32, i32* %19, align 4

    %22 = add i32 %20, %21
    %23 = alloca i32, align 4
    store i32 %22, i32* %23, align 4 ; storing the answer

    store i32 %22, i32* %1, align 4

    %24 = alloca i32, align 4
    store i32 1, i32* %24, align 4

    %25 = getelementptr i32, i32* %2, i32 0

    ; loading values for binary operation

    %26 = load i32, i32* %24, align 4

    %27 = load i32, i32* %25, align 4

    %28 = add i32 %26, %27
    %29 = alloca i32, align 4
    store i32 %28, i32* %29, align 4 ; storing the answer

    store i32 %28, i32* %2, align 4

    %30 = alloca i32, align 4
    store i32 1, i32* %30, align 4

    %31 = getelementptr i32, i32* %3, i32 0

    ; loading values for binary operation

    %32 = load i32, i32* %30, align 4

    %33 = load i32, i32* %31, align 4

    %34 = add i32 %32, %33
    %35 = alloca i32, align 4
    store i32 %34, i32* %35, align 4 ; storing the answer

    store i32 %34, i32* %3, align 4

    %36 = alloca i32, align 4
    store i32 1, i32* %36, align 4

    %37 = getelementptr i32, i32* %1, i32 0

    ; loading values for binary operation

    %38 = load i32, i32* %36, align 4

    %39 = load i32, i32* %37, align 4

    %40 = add i32 %38, %39
    %41 = alloca i32, align 4
    store i32 %40, i32* %41, align 4 ; storing the answer

    store i32 %40, i32* %1, align 4

    %42 = load i32, i32* %41, align 4 ;loading return value 
    ret i32 %42

}