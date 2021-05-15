//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Block.h
//
// Eric Meehan
// 4/10/21
//
//

#ifndef OBJ_Block_h
#define OBJ_Block_h

#include "../blockchain.h"
#include "../objects/Account.h"

typedef struct BlockHeaders
{
    // A reference to another block in the chain.
    byte previous_hash[64];
    // A timestamp to designate when the block was created (seconds since epoch UTC)
    byte timestamp[20];
    // The size of the block in bytes
    unsigned long size;
    // The creator's public key.
    byte key[550];
    // A digital signature for the data.
    byte lock[512];
    // A nonce to satisfy the difficulty requirement.
    unsigned long nonce;
}
__attribute__((packed))
BLOCKCHAIN_OBJ_BlockHeaders;


typedef struct Block
{
    BLOCKCHAIN_OBJ_BlockHeaders headers;
    byte data;
}
__attribute__((packed))
BLOCKCHAIN_OBJ_Block;


bool BLOCKCHAIN_OBJ_Block_mine(BLOCKCHAIN_OBJ_Account *user, BLOCKCHAIN_OBJ_Block *previous, void *data, unsigned long size, byte *hash);
bool BLOCKCHAIN_OBJ_Block_validate(BLOCKCHAIN_OBJ_Block *block);
bool BLOCKCHAIN_OBJ_Block_hash(BLOCKCHAIN_OBJ_Block *block, byte *digest);
bool BLOCKCHAIN_OBJ_Block_load(BLOCKCHAIN_OBJ_Block *block, byte *address);

#endif /* OBJ_Block_h */
