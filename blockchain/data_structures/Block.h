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

typedef unsigned char byte;

struct Incidentals
{
    // A reference to another block in the chain.
    byte previous_hash[64];
    // A timestamp to designate when the block was created (seconds since epoch UTC)
    byte timestamp[32];
    // The target is the largest possible hash value for this block
    byte target[64];
}__attribute__((packed));

struct Credentials
{
    // The creator's public key.
    byte key[550];
    // A digital signature for the data.
    byte lock[512];
    // A nonce to satisfy the difficulty requirement.
    byte nonce[32];
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

#endif /* Block_h */
