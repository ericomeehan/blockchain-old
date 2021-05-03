//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// template.c
//
// Eric Meehan
// 5/2/21
//
//

#include <stdio.h>

int first_test_case(void);
float test(void);

float test()
{
    // This function calls all of the subsequent test functions and tracks their success/failure.
    const int num_tests = 1;
    int successful_tests = 0;
    
    successful_tests += first_test_case();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

int first_test_case()
{
    return 1;
}
