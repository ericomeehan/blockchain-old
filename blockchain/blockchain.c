//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// blockchain.c
//
// Eric Meehan
// 5/29/21
//
//

#include "blockchain.h"

int BLOCKCHAIN_account_create(char *username)
{
    EVP_PKEY *key = EVP_PKEY_new();
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 4096);
    EVP_PKEY_keygen(ctx, &key);
    
    char path[64] = {0};
    strcat(path, PROFILE_PATH);
    strcat(path, username);
    strcat(path, ".pem");
    
    FILE *key_file = fopen(path, "w");
    if (!PEM_write_PrivateKey(key_file, key, EVP_aes_256_cbc(), NULL, 0, NULL, NULL))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to write private key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    fclose(key_file);
    
    memset(path, 0, 64);
    strcat(path, PROFILE_PATH);
    strcat(path, username);
    strcat(path, ".der");
    
    key_file = fopen(path, "w");
    
    if (!i2d_PUBKEY_fp(key_file, key))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to write public key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    fclose(key_file);
    EVP_PKEY_CTX_free(ctx);
    return EXIT_SUCCESS;
}

int BLOCKCHAIN_account_login(BLOCKCHAIN_OBJ_Account *account, char *username)
{
    account->public_key = EVP_PKEY_new();                               // REMEMBER TO FREE THESE
    account->private_key = EVP_PKEY_new();
    
    char path[64] = {0};
    strcat(path, PROFILE_PATH);
    strcat(path, username);
    strcat(path, ".der");
    
    FILE *key_file = fopen(path, "r");
    if (!key_file)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Could not find public key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_free(account->private_key);
        EVP_PKEY_free(account->public_key);
        return EXIT_FAILURE;
    }
    
    if (!d2i_PUBKEY_fp(key_file, &account->public_key))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load public key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_free(account->private_key);
        EVP_PKEY_free(account->public_key);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "BLOCKCHAIN: Successfully loaded public key for account %s\n", username);
    fclose(key_file);
    
    memset(path, 0, 64);
    strcat(path, PROFILE_PATH);
    strcat(path, username);
    strcat(path, ".pem");
    
    key_file = fopen(path, "r");
    if (!key_file)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Could not find private key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_free(account->private_key);
        EVP_PKEY_free(account->public_key);
        return EXIT_FAILURE;
    }
    
    if (!PEM_read_PrivateKey(key_file, &account->private_key, NULL, NULL))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load private key for account %s\n", username);
        fclose(key_file);
        EVP_PKEY_free(account->private_key);
        EVP_PKEY_free(account->public_key);
        return EXIT_FAILURE;
    }
    
    fprintf(stdout, "BLOCKCHAIN: Successfully loaded private key for account %s\n", username);
    return EXIT_SUCCESS;
}

int BLOCKCHAIN_block_hash(BLOCKCHAIN_OBJ_Block *block, byte *digest)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(ctx, EVP_sha512(), NULL) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to initialize EVP context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    if (EVP_DigestUpdate(ctx, block, block->headers.size) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to update EVP context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    unsigned int size = 64;
    if (EVP_DigestFinal(ctx, digest, &size) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to finalize EVP context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    EVP_MD_CTX_free(ctx);
    return EXIT_SUCCESS;
}

int BLOCKCHAIN_block_calculate_difficulty(BLOCKCHAIN_OBJ_Block *block)
{
    return pow(log(sqrt((double)block->headers.size)), 2) + 1;
}

bool BLOCKCHAIN_block_proof_of_work(BLOCKCHAIN_OBJ_Block *block)
{
    BN_CTX *ctx = BN_CTX_new();
    
    BIGNUM *base = BN_new();
    BN_dec2bn(&base, "2");
    
    BIGNUM *exponent = BN_new();
    int difficulty = 512 - BLOCKCHAIN_block_calculate_difficulty(block);
    char exp[8] = {0};
    sprintf(exp, "%d", difficulty);
    BN_dec2bn(&exponent, exp);
    
    BIGNUM *target = BN_new();
    BN_exp(base, exponent, target, ctx);
    
    byte digest[64];
    BLOCKCHAIN_block_hash(block, digest);
    BIGNUM *value = BN_new();
    BN_bin2bn(digest, 64, value);
    
    bool result = BN_cmp(target, value) == 1;
    
    BN_free(base);
    BN_free(exponent);
    BN_free(target);
    BN_free(value);
    BN_CTX_free(ctx);
    
    return result;
}


int BLOCKCHAIN_block_create(BLOCKCHAIN_OBJ_Account *account, BLOCKCHAIN_OBJ_Block *previous, byte *data, unsigned long size)
{
    BLOCKCHAIN_OBJ_Block *block = malloc(sizeof(BLOCKCHAIN_OBJ_BlockHeaders) + size);
    
    i2d_PUBKEY(account->public_key, (unsigned char **)&block->headers.author);
    block->headers.size = sizeof(BLOCKCHAIN_OBJ_BlockHeaders) + size;
    memcpy(&block->data, data, size);
    BLOCKCHAIN_block_sign(account, block);
    
    if (BLOCKCHAIN_block_hash(previous, block->headers.previous) != EXIT_SUCCESS)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to hash previous block\n");
        free(block);
        return EXIT_FAILURE;
    }
    
    block->headers.nonce = 0;
    while (!BLOCKCHAIN_block_proof_of_work(block)) { block->headers.nonce += 1; }
    
    
}

int BLOCKCHAIN_block_sign(BLOCKCHAIN_OBJ_Account *account, BLOCKCHAIN_OBJ_Block *block)
{
    memset(block->headers.signature, 0, 512);
    
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    
    if (EVP_DigestSignInit(ctx, NULL, EVP_sha512(), NULL, account->private_key) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to initialize signature context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    if (EVP_DigestSignUpdate(ctx, &block->data, block->headers.size - sizeof(BLOCKCHAIN_OBJ_BlockHeaders)) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to update signature context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    size_t siglen;
    if (EVP_DigestSignFinal(ctx, block->headers.signature, &siglen) <= 0)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to initialize signature context\n");
        EVP_MD_CTX_free(ctx);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "BLOCKCHAIN: Successfully signed block\n");
    EVP_MD_CTX_free(ctx);
    return EXIT_SUCCESS;
}
