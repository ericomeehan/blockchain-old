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

#include <string.h>

bool test_successful_mine(void);
bool test_unsuccessful_mine(void);
bool test_successful_validate(void);
bool test_unsuccessful_validate(void);


float block_unit_test()
{
    activate("test");
    const float num_tests = 2;
    float successful_tests = 0;
    
    successful_tests += test_successful_mine();
    successful_tests += test_successful_validate();
    
    float rate = successful_tests / num_tests;
    
    return rate;
}

bool test_successful_mine()
{
    byte data[12] = "hello world";
    byte digest[64];
    return mine(NULL, data, 12, digest);
}

bool test_unsuccessful_mine()
{
    byte data[12] = "hello world";
    byte digest[64];
    return !mine(NULL, data, 12, digest);
}


bool test_successful_validate()
{
    byte data[12] = "hello world";
    byte digest[64];
    
    mine(NULL, data, 12, digest);
    
    struct Block block;
    load_block(&block, digest);
    
    return validate(&block);
}
