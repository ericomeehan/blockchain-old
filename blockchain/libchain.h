//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// libchain.h
//
// Eric Meehan
// 5/12/21
//
//

#ifndef libchain_h
#define libchain_h

#include "objects/Account.h"
#include "objects/Block.h"

#include "interfaces/account.h"
#include "interfaces/create.h"
#include "interfaces/query.h"
#include "interfaces/read.h"
#include "interfaces/server.h"
#include "interfaces/share.h"

#define BLOCKCHAIN_FTND_account account
#define BLOCKCHAIN_FTND_create create
#define BLOCKCHAIN_FTND_read read
#define BLOCKCHAIN_FTND_server server
#define BLOCKCHAIN_FTND_share share

#define BLOCKCHAIN_BKND_ACCOUNT_create create_account
#define BLOCKCHAIN_BKND_ACCOUNT_activate activate_account
#define BLOCKCHAIN_BKND_ACCOUNT_deactivate deactivate_account
#define BLOCKCHAIN_BKND_ACCOUNT_print print_account

#define BLOCKCHAIN_BKND_BLOCK_mine mine_block
#define BLOCKCHAIN_BKND_BLOCK_validate validate_block
#define BLOCKCHAIN_BKND_BLOCK_hash hash_block

#define BLOCKCHAIN_TYPE_Account Account
#define BLOCKCHAIN_TYPE_Block Block

#define BLOCKCHAIN_SERVER server

#endif /* libchain_h */
