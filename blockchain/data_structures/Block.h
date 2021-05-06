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

#include <libeom/libeom.h>
#include "Account.h"

static char *block_path = "/Users/eric/Desktop/";

struct Incidentals
{
    // The hash for this block
    byte hash[64];
    // A reference to another block in the chain.
    byte previous_hash[64];
    // A timestamp to designate when the block was created (seconds since epoch UTC)
    byte timestamp[32];
    // The size of the block in bytes
    unsigned long long size;
}__attribute__((packed)); 

struct Credentials
{
    // The creator's public key.
    byte key[550];
    // A digital signature for the data.
    byte lock[512];
    // A nonce to satisfy the difficulty requirement.
    unsigned long long nonce;
}__attribute__((packed));

struct BlockHeaders
{
    struct Incidentals incidentals;
    struct Credentials credentials;
}__attribute__((packed));

struct Block
{
    struct BlockHeaders headers;
    byte data;
}__attribute__((packed));


bool mine(struct Block *previous, void *data, unsigned long size, byte *hash);
bool validate(struct Block *block);
bool load_block(struct Block *block, byte *address);

#endif /* Block_h */
