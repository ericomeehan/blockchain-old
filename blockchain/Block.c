//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Block.c
//
// Eric Meehan
// 4/10/21
//
//

#include "Block.h"
#include <stdlib.h>
#include <string.h>

struct Block * block_constructor(unsigned char previous_hash[64], unsigned long nonce, void *data, unsigned long size)
{
    struct Block *block = calloc(1, sizeof(struct BlockHeaders) + size);
    for (int i = 0; i < 64; i ++)
    {
        block->headers.previous_hash[i] = previous_hash[i];
    }
    block->headers.nonce = nonce;
    block->headers.size = sizeof(struct BlockHeaders) + size;
    
    memcpy(&block->data, data, size);
    
    return block;
}
