//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// test_account.c
//
// Eric Meehan
// 5/5/21
//
//

#include "test_account.h"

#include "Account.h"

#include <stdio.h>

bool test_successful_create(void);
bool test_unsuccessful_create(void);
bool test_successful_activate(void);
bool test_unsuccessful_activate(void);
bool test_successful_deacitvate(void);


float account_unit_test()
{
    // This function calls all of the subsequent test functions and tracks their success/failure.
    const float num_tests = 5;
    float successful_tests = 0;
    
    successful_tests += test_successful_create();
    successful_tests += test_unsuccessful_create();
    successful_tests += test_successful_activate();
    successful_tests += test_unsuccessful_activate();
    successful_tests += test_successful_deacitvate();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

bool test_successful_create()
{
    return create("test");
}

bool test_unsuccessful_create()
{
    // The test account should already exits.
    return !create("test");
}

bool test_successful_activate(void)
{
    return activate("test");
}

bool test_unsuccessful_activate(void)
{
    return !activate("NA");
}

bool test_successful_deacitvate(void)
{
    return deactivate();
}

