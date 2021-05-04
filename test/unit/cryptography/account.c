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
int test_sign(void);
int test_verify(void);

float test(void);

static struct Account account;

float test()
{
    // This function calls all of the subsequent test functions and tracks their success/failure.
    const float num_tests = 3;
    float successful_tests = 0;
    
    successful_tests += test_activate_account();
    successful_tests += test_sign();
    successful_tests += test_verify();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

int test_activate_account()
{
    account = activate("/Users/eric/Desktop/eric.der", "/Users/eric/Desktop/eric.pem");
    return account.private_key && account.public_key;
}

int test_sign()
{
    FILE *f = fopen("/Users/eric/Desktop/test.txt", "r");
    unsigned char data[12];
    fread(data, 1, 12, f);
    fclose(f);
    
    unsigned char* encMessage;
    size_t encMessageLength;
    
    short i = RSASign(account.private_key, data, 12, &encMessage, &encMessageLength);
    
    f = fopen("/Users/eric/Desktop/signature", "w");
    fwrite(encMessage, encMessageLength, 1, f);
    fclose(f);
    
    return i;
}

int test_verify()
{
    FILE *f = fopen("/Users/eric/Desktop/test.txt", "r");
    unsigned char data[12];
    fread(data, 1, 12, f);
    fclose(f);
    
    unsigned char* encMessage;
    size_t encMessageLength;
    
    
    RSASign(account.private_key, data, 12, &encMessage, &encMessageLength);
    
    short tru;
    tru = RSAVerifySignature(account.public_key, encMessage, encMessageLength, (const char *)data, 12, &tru);
    return tru;
}
