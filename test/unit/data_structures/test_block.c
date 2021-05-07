//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// block.c
//
// Eric Meehan
// 5/4/21
//
//

#include "test_block.h"

#include <openssl/pem.h>

#include <string.h>

bool test_successful_mine(Account *user);
bool test_unsuccessful_mine(void);
bool test_successful_validate(Account *user);
bool test_unsuccessful_validate(void);


float block_unit_test(Account *user)
{
    const float num_tests = 2;
    float successful_tests = 0;
    
    successful_tests += test_successful_mine(user);
    successful_tests += test_successful_validate(user);
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

bool test_successful_mine(Account *user)
{
    byte data[12] = "hello world";
    byte digest[64];
    return mine(user, NULL, data, 12, digest);
}

bool test_unsuccessful_mine()
{
    Account user;
    byte data[12] = "hello world";
    byte digest[64];
    return !mine(&user, NULL, data, 12, digest);
}


bool test_successful_validate(Account *user)
{
    byte data[12] = "hello world";
    byte digest[64];
    
    mine(user, NULL, data, 12, digest);
    
    BIGNUM *hash = BN_new();
    BN_bin2bn(digest, 64, hash);
    
    struct Block block;
    load_block(&block, (byte *)BN_bn2hex(hash));
    
    return validate(&block);
}
