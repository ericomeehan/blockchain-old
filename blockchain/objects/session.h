//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// OBJ_session.h
//
// Eric Meehan
// 5/15/21
//
//

#ifndef OBJ_session_h
#define OBJ_session_h

#include "../blockchain.h"

#include "account.h"
#include "block.h"

typedef struct LinkedBlock
{
    BLOCKCHAIN_OBJ_Block *previous;
    BLOCKCHAIN_OBJ_Block *block;
}
BLOCKCHAIN_OBJ_LinkedBlock;
void BLOCKCHAIN_OBJ_LinkedBlock_next(BLOCKCHAIN_OBJ_LinkedBlock *linked_block);

typedef struct Session
{
    BLOCKCHAIN_OBJ_Account user;
    BLOCKCHAIN_OBJ_Block *whoami;
    BLOCKCHAIN_OBJ_LinkedBlock events;
}
BLOCKCHAIN_OBJ_Session;

#endif /* OBJ_session_h */
