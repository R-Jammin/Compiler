#include <stdio.h>

extern "C" 
{
    int myFunctionFinal(int argA);
}

int main()
{
   int argA = 5;
   printf("\nThe Answer = %d\n\n", myFunctionFinal(argA));
}