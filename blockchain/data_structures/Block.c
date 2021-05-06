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

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

bool sign_block(struct Block *block);
bool validate_signature(struct Block *block);
bool hash_block(struct Block *block, byte *digest);
bool validate_hash(struct Block *block);

bool mine(struct Block *previous, void *data, unsigned long size, byte *digest)
{
    // Allocate space for the block
    unsigned long block_size = size + sizeof(struct BlockHeaders);
    struct Block *block = malloc(block_size);
    // Initialize some of the fields
    block->headers.credentials.nonce = 0;
    
    // Insert the incidentals and data
    block->headers.incidentals.size = block_size;
    memcpy(&block->data, data, size);
    //memcpy(block->headers.incidentals.previous_hash, previous, size)
    
    if (!sign_block(block))
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
    
    char *path = "";
    strcat(path, block_path);
    strcat(path, BN_bn2hex(value));
    strcat(path, ".block");
    FILE *block_file = fopen(path, "w");
    fwrite(block, block->headers.incidentals.size, 1, block_file);
    fclose(block_file);
    
    free(block);
    
    return true;
}
bool validate(struct Block *block)
{
    return validate_hash(block) && validate_signature(block);
}

bool sign_block(struct Block *block)
{
    // Embed the active user's public key in the block.
    i2d_PUBKEY(user.public_key, (unsigned char **)&block->headers.credentials.key);
    // Collect the size of the data embedded within the block.
    unsigned long long size = block->headers.incidentals.size - sizeof(struct BlockHeaders);
    // Prepare an EVP control structure
    EVP_MD_CTX *ctx = NULL;
    ctx = EVP_MD_CTX_create();

    // Create a signature for the block, checking for errors.
    if ((EVP_DigestSignInit(ctx, NULL, EVP_sha512(), NULL, user.private_key)) <= 0)
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
    return true;
}


bool validate_signature(struct Block *block)
{
    // Prepare an EVP control structure.
    EVP_MD_CTX *ctx = EVP_MD_CTX_create();
    // Load the embedded public key as an EVP_PKEY.
    EVP_PKEY *key = NULL;
    d2i_PUBKEY(&key, (const unsigned char **)&block->headers.credentials.key, 550);
    // Verify the signature, checking for errors.
    byte *hash;
    unsigned long hash_length = 0;
    
    if (EVP_DigestVerifyInit(ctx, NULL, EVP_sha512(), NULL, key) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyUpdate(ctx, &block->data, block->headers.incidentals.size - sizeof(struct BlockHeaders)) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyFinal(ctx, hash, hash_length) <= 0)
    {
        return false;
    }
    // Confirm the signature matches the hash
    return EVP_DigestVerifyFinal(ctx, hash, hash_length) == 1;
}

bool hash_block(struct Block *block, byte *digest)
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
    
    return true;
}

bool validate_hash(struct Block *block)
{
    // Create a new bignum control structure
    BN_CTX *ctx = BN_CTX_new();
    
    // The target hash value is 2^(512-difficulty).
    // This needs to be represented using the OpenSSL bignum library.
    BIGNUM *base = BN_new();
    BN_dec2bn(&base, "2");
    
    BIGNUM *exponent = BN_new();
    // The difficulty and value of a block is equal to 10 * √log(block_size) + 1.
    // The reasoning behind this formula is clear when graphed.
    // Truncate the difficulty to an intiger to avoid contentious precision.
    int difficulty = 512 - (10 * sqrt(log(block->headers.incidentals.size)) + 1);
    char exp[8];
    sprintf(exp, "%d", difficulty);
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

bool load_block(struct Block *block, byte *address)
{
    char path[64] = {0};
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
