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

#include <openssl/rsa.h>

struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
};

void create_account(void);
struct Account activate(char *public_key_file, char *private_key_file);
void deactivate(void);

#endif /* Account_h */
