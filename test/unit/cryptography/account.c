//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// account.c
//
// Eric Meehan
// 5/2/21
//
//

#include <stdio.h>

#include "../../../blockchain/cryptography/Account.h"

int test_activate_account(void);

float test(void);

float test()
{
    // This function calls all of the subsequent test functions and tracks their success/failure.
    const int num_tests = 1;
    int successful_tests = 0;
    
    successful_tests += test_activate_account();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

int test_activate_account()
{
    struct Account account = activate("/Users/eric/Desktop/eric_public.pem", "/Users/eric/Desktop/eric_private.pem");
    return account.private_key && account.public_key;
}
