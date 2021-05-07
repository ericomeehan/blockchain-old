//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Account.c
//
// Eric Meehan
// 5/2/21
//
//

#include "Account.h"

#include <string.h>
#include <unistd.h>

#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

bool create(Account *user, char *name)
{
    EVP_PKEY *key = EVP_PKEY_new();
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 4096);
    EVP_PKEY_keygen(ctx, &key);
    
    char path[64] = {0};
    strcat(path, profile_path);
    strcat(path, name);
    strcat(path, ".der");
    
    FILE *key_file = fopen(path, "r");
    if (key_file)
    {
        return false;
    }
    fclose(key_file);
    
    key_file = fopen(path, "w");
    i2d_PrivateKey_fp(key_file, key);
    fclose(key_file);
    
    memset(path, 0, 64);
    strcat(path, profile_path);
    strcat(path, name);
    strcat(path, "_public.der");
    
    key_file = fopen(path, "w");
    i2d_PUBKEY_fp(key_file, key);
    fclose(key_file);
    
    EVP_PKEY_CTX_free(ctx);
    
    return activate(user, name);
}

bool activate(Account *user, char *name)
{
    char path[64] = {0};
    strcat(path, profile_path);
    strcat(path, name);
    strcat(path, "_public.der");
    FILE *key_file = fopen(path, "r");
    if (!key_file)
    {
        return false;
    }
    user->public_key = EVP_PKEY_new();
    d2i_PUBKEY_fp(key_file, &user->public_key);
    fclose(key_file);
    
    memset(path, 0, 64);
    strcat(path, profile_path);
    strcat(path, name);
    strcat(path, ".der");
    key_file = fopen(path, "r");
    if (!key_file)
    {
        return false;
    }
    user->private_key = EVP_PKEY_new();
    user->private_key = d2i_PrivateKey_ex_fp(key_file, &user->private_key, NULL, NULL);
    fclose(key_file);
    
    return user->public_key && user->private_key;
}

bool deactivate(Account *user)
{
    
    user->public_key = NULL;
    user->private_key = NULL;
    
    return !user->public_key && !user->private_key;
}
