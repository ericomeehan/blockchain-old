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

struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
};

static const char *profile_path = "/Users/eric/Desktop/";
static struct Account user;

bool create(char *name);
bool activate(char *name);
bool deactivate(void);

#endif /* Account_h */
