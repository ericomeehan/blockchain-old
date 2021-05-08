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

#include <openssl/pem.h>

bool test_successful_create(void);
bool test_unsuccessful_create(void);
bool test_successful_activate(void);
bool test_unsuccessful_activate(void);
bool test_successful_deacitvate(void);


float account_unit_test()
{
    // This function calls all of the subsequent test functions and tracks their success/failure.
    const float num_tests = 4;
    float successful_tests = 0;
    
    //successful_tests += test_successful_create();
    successful_tests += test_unsuccessful_create();
    successful_tests += test_successful_activate();
    successful_tests += test_unsuccessful_activate();
    successful_tests += test_successful_deacitvate();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

bool test_successful_create()
{
    Account test;
    if (!create(&test, "test"))
    {
        return false;
    }
    else
    {
        return test.private_key && test.public_key;
    }
}

bool test_unsuccessful_create()
{
    // The test account should already exits.
    Account test;
    return !create(&test, "test");
}

bool test_successful_activate(void)
{
    Account test;
    return activate(&test, "test");
}

bool test_unsuccessful_activate(void)
{
    Account test;
    return !activate(&test, "NA");
}

bool test_successful_deacitvate(void)
{
    Account test;
    return deactivate(&test);
}

