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

struct BlockHeaders
{
    unsigned char previous_hash[64];
    unsigned long nonce;
    unsigned long size;
}__attribute__((packed));

struct Block
{
    struct BlockHeaders headers;
    unsigned char data;
}__attribute__((packed));

struct Block * block_constructor(unsigned char previous_hash[64], unsigned long nonce, void *data, unsigned long size);

#endif /* Block_h */
