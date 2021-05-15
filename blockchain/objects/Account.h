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

#ifndef OBJ_Account_h
#define OBJ_Account_h

#include "../blockchain.h"

typedef struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
}
BLOCKCHAIN_OBJ_Account;

bool BLOCKCHAIN_OBJ_Account_create(BLOCKCHAIN_OBJ_Account *user, char *name);
bool BLOCKCHAIN_OBJ_Account_activate(BLOCKCHAIN_OBJ_Account *user, char *name);
bool BLOCKCHAIN_OBJ_Account_deactivate(BLOCKCHAIN_OBJ_Account *user);
void BLOCKCHAIN_OBJ_Account_print(BLOCKCHAIN_OBJ_Account *user);
bool BLOCKCHAIN_OBJ_Account_login(BLOCKCHAIN_OBJ_Account *user);

#endif /* OBJ_Account_h */
