//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// blockchain.h
//
// Eric Meehan
// 5/29/21
//
//

#ifndef blockchain_h
#define blockchain_h

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

const char *PROFILE_PATH = "/Users/eric/Desktop/";

typedef unsigned char byte;

typedef struct BLOCKCHAIN_OBJ_BlockHeaders
{
    byte author[550];
    byte signature[512];
    unsigned long nonce;
    unsigned long size;
    byte previous[64];
} __attribute__((packed)) BLOCKCHAIN_OBJ_BlockHeaders;
 
typedef struct BLOCKCHAIN_OBJ_Block
{
    BLOCKCHAIN_OBJ_BlockHeaders headers;
    byte data;
} __attribute__((packed)) BLOCKCHAIN_OBJ_Block;

typedef struct BLOCKCHAIN_OBJ_Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
} BLOCKCHAIN_OBJ_Account;

int BLOCKCHAIN_block_create(BLOCKCHAIN_OBJ_Account *account, BLOCKCHAIN_OBJ_Block *previous, byte *data, unsigned long size);
int BLOCKCHAIN_block_hash(BLOCKCHAIN_OBJ_Block *block, byte *digtest);
bool BLOCKCHAIN_block_proof_of_work(BLOCKCHAIN_OBJ_Block *block);
int BLOCKCHAIN_block_calculate_difficulty(BLOCKCHAIN_OBJ_Block *block);
int BLOCKCHAIN_block_sign(BLOCKCHAIN_OBJ_Account *account, BLOCKCHAIN_OBJ_Block *block)

int BLOCKCHAIN_account_create(char *username);
int BLOCKCHAIN_account_login(BLOCKCHAIN_OBJ_Account *account, char *username);

#endif /* blockchain_h */
