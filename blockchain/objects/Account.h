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

#include "../blockchain.h"

typedef struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
} Account;

bool create_account(Account *user, char *name);
bool activate_account(Account *user, char *name);
bool deactivate_account(Account *user);
void print_account(Account *user);

#endif /* Account_h */
