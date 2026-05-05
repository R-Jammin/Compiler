#include <stdio.h>

extern "C" 
{
    int myFunctionFinal();
}

int main()
{
    printf("\nThe Answer = %d\n\n", myFunctionFinal());
}