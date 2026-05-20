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

    %6 = load i32, i32* %5, align 4 ;loading return value 
    ret i32 %6

}