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

#include "../main.h"
#include "../accounts/Account.h"

struct Incidentals
{
    // A reference to another block in the chain.
    byte previous_hash[64];
    // A timestamp to designate when the block was created (seconds since epoch UTC)
    byte timestamp[20];
    // The size of the block in bytes
    unsigned long size;
}__attribute__((packed)); 

struct Credentials
{
    // The creator's public key.
    byte key[550];
    // A digital signature for the data.
    byte lock[512];
    // A nonce to satisfy the difficulty requirement.
    unsigned long nonce;
}__attribute__((packed));

typedef struct BlockHeaders
{
    struct Incidentals incidentals;
    struct Credentials credentials;
}__attribute__((packed)) BlockHeaders;

typedef struct Block
{
    BlockHeaders headers;
    byte data;
}__attribute__((packed)) Block;


bool mine(Account *user, Block *previous, void *data, unsigned long size, byte *hash);
bool validate(Block *block);
bool hash_block(Block *block, byte *digest);
bool load_block(Block *block, byte *address);

#endif /* Block_h */
