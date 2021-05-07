//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Account.h
//
// Eric Meehan
// 5/2/21
//
//

#ifndef Account_h
#define Account_h

#include <libeom/libeom.h>

#include <openssl/evp.h>

typedef struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
} Account;

static const char *profile_path = "/Users/eric/Desktop/";

bool create(Account *user, char *name);
bool activate(Account *user, char *name);
bool deactivate(Account *user);

#endif /* Account_h */
