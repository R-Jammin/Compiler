#include <stdio.h>
#include <stdlib.h>

extern "C" 
{
    int myFunction(int argA);
    int myFunctionFinal(int argA);
}

int main()
{
    int argA = 5;   

    printf("\nThe answer should be: %d\n", (unsigned int)myFunction(argA));
    printf("The given answer was: %d\n", (unsigned int)myFunctionFinal(argA));

    unsigned int baseline = myFunction(argA);
    unsigned int test = myFunctionFinal(argA);
    
    if (baseline == test)
    {
        printf("\nTEST PASSED!\n\n");

        return 0; //passed
    }
    else
    {
        printf("\n***** TEST FAILED! *****\n\n");

        return -1; //failed
    }

    return -1;
}