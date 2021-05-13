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

#ifndef Block_h
#define Block_h

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
}__attribute__((packed)) BlockHeaders;

typedef struct Block
{
    BlockHeaders headers;
    byte data;
}__attribute__((packed)) Block;


bool mine_block(Account *user, Block *previous, void *data, unsigned long size, byte *hash);
bool validate_block(Block *block);
bool hash_block(Block *block, byte *digest);
bool load_block(Block *block, byte *address);

#endif /* Block_h */
