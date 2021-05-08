//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Block.c
//
// Eric Meehan
// 4/10/21
//
//

#include "Block.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

bool timestamp(Block *block);
bool sign_block(Account *user, Block *block);
bool validate_signature(Block *block);
bool hash_block(Block *block, byte *digest);
bool validate_hash(Block *block);

bool mine(Account *user, Block *previous, void *data, unsigned long size, byte *digest)
{
    FILE *f = fopen("/Users/eric/Desktop/test", "w");
    i2d_PUBKEY_fp(f, user->public_key);
    fclose(f);
    // Allocate space for the block
    unsigned long block_size = size + sizeof(BlockHeaders);
    Block *block = malloc(block_size);
    
    // Initialize some of the fields
    block->headers.credentials.nonce = 0;
    if (!timestamp(block))
    {
        return false;
    }
    
    // Insert the incidentals and data
    block->headers.incidentals.size = block_size;
    memcpy(&block->data, data, size);
    //memcpy(block->headers.incidentals.previous_hash, previous, size)
    
    if (!sign_block(user, block))
    {
        return false;
    }
    while (!validate_hash(block))
    {
        block->headers.credentials.nonce += 1;
    }
    if (!validate_hash(block))
    {
        return false;
    }
    
    // Write the new block to a file using its hash as a name
    hash_block(block, digest);
    BIGNUM *value = BN_new();
    BN_bin2bn(digest, 64, value);
    
    char path[256] = {0};
    strcat(path, block_path);
    strcat(path, BN_bn2hex(value));
    strcat(path, ".block");
    FILE *block_file = fopen(path, "w");
    fwrite(block, block->headers.incidentals.size, 1, block_file);
    fclose(block_file);
    
    free(block);
    BN_free(value);
    
    return true;
}
bool validate(Block *block)
{
    return validate_hash(block) && validate_signature(block);
}

bool sign_block(Account *user, Block *block)
{
    // Embed the active user's public key in the block.
    memset(&block->headers.credentials.key, 0, 550);
    i2d_PUBKEY(user->public_key, (unsigned char **)&block->headers.credentials.key);
    // Collect the size of the data embedded within the block.
    unsigned long long size = block->headers.incidentals.size - sizeof(struct BlockHeaders);
    // Prepare an EVP control structure
    EVP_MD_CTX *ctx = NULL;
    ctx = EVP_MD_CTX_create();

    // Create a signature for the block, checking for errors.
    if ((EVP_DigestSignInit(ctx, NULL, EVP_sha512(), NULL, user->private_key)) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignUpdate(ctx, &block->data, size) <= 0)
    {
        return false;
    }
    size_t len;
    if (EVP_DigestSignFinal(ctx, NULL, &len) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignFinal(ctx, (unsigned char *)&block->headers.credentials.lock, &len) <= 0)
    {
        return false;
    }
    EVP_MD_CTX_free(ctx);
    return true;
}


bool validate_signature(Block *block)
{
    // Prepare an EVP control structure.
    EVP_MD_CTX *ctx = EVP_MD_CTX_create();
    // Load the embedded public key as an EVP_PKEY.
    EVP_PKEY *key = NULL;
    d2i_PUBKEY(&key, (const unsigned char **)&block->headers.credentials.key, 550);
    // Verify the signature, checking for errors.
    
    if (EVP_DigestVerifyInit(ctx, NULL, EVP_sha512(), NULL, key) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyUpdate(ctx, &block->data, block->headers.incidentals.size - sizeof(struct BlockHeaders)) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyFinal(ctx, block->headers.credentials.lock, 512) <= 0)
    {
        return false;
    }
    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(key);
    return true;
}

bool hash_block(Block *block, byte *digest)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    
    if (EVP_DigestInit_ex(ctx, EVP_sha512(), NULL) <= 0)
    {
        return false;
    }
    if (EVP_DigestUpdate(ctx, block, block->headers.incidentals.size) <= 0)
    {
        return false;
    }
    unsigned int size = 64;
    if (EVP_DigestFinal(ctx, digest, &size) <= 0)
    {
        return false;
    }
    EVP_MD_CTX_free(ctx);
    return true;
}

bool validate_hash(Block *block)
{
    // Create a new bignum control structure
    BN_CTX *ctx = BN_CTX_new();
    
    // The target hash value is 2^(512-difficulty).
    // This needs to be represented using the OpenSSL bignum library.
    BIGNUM *base = BN_new();
    BN_dec2bn(&base, "2");
    
    BIGNUM *exponent = BN_new();
    // Truncate the difficulty to an intiger to avoid contentious precision.
    int difficulty = 512 - pow(log(sqrt(block->headers.incidentals.size)), 2) + 1;
    char exp[8];
    sprintf(exp, "%df", difficulty);
    BN_dec2bn(&exponent, exp);
    
    BIGNUM *target = BN_new();
    BN_exp(target, base, exponent, ctx);
    
    BIGNUM *hash = BN_new();
    byte digest[64];
    hash_block(block, digest);
    BN_bin2bn(digest, 64, hash);
    
    // If the block is valid, its hash will be less than the target.
    int proof_of_work = BN_cmp(target, hash);
    
    // Clean up after OpenSSL
    BN_free(base);
    BN_free(exponent);
    BN_free(target);
    BN_free(hash);
    BN_CTX_free(ctx);
    
    return proof_of_work == 1;
}

bool load_block(Block *block, byte *address)
{
    char path[256] = {0};
    strcat(path, block_path);
    strcat(path, (char *)address);
    strcat(path, ".block");
    FILE *f = fopen(path, "r");
    if (!f)
    {
        return false;
    }

    fread(block, sizeof(struct BlockHeaders), 1, f);
    fread(&block->data, block->headers.incidentals.size - sizeof(struct BlockHeaders), 1, f);
    
    fclose(f);
    return true;
}

bool timestamp(Block *block)
{
    struct tm *raw_time;
    time_t time_object = time(NULL);
    raw_time = gmtime(&time_object);
    char timestamp[20] = {0};
    sprintf(timestamp, "%d-%02d-%02d %02d:%02d:%02d", raw_time->tm_year + 1900, raw_time->tm_mon + 1, raw_time->tm_mday, raw_time->tm_hour, raw_time->tm_min, raw_time->tm_sec);
    memcpy(block->headers.incidentals.timestamp, timestamp, 20);
    return true;
}
