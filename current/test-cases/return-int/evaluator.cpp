#include <stdio.h>

extern "C" 
{
    int myFunction();
    int myFunctionFinal();
}

int main()
{
    
    printf("\nThe answer should be: %d\n", (unsigned int)myFunction());
    printf("The given answer was: %d\n", (unsigned int)myFunctionFinal());

    unsigned int baseline = myFunction();
    unsigned int test = myFunctionFinal();
    
    if (baseline == test)
    {
        printf("\nTEST PASSED!\n\n");

        return 0; //passed
    }
    else
    {
        printf("\n***** TEST FAILED! *****\n\n");

        return 1; //failed
    }

    return 1;
}