//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Reader.h
//
// Eric Meehan
// 4/10/21
//
//

#ifndef Reader_h
#define Reader_h

#include <libeom/libeom.h>

#include <stdio.h>

#include "Block.h"

void *reader(char *block)
{
    return read_file(block) + sizeof(struct BlockHeaders);
}

#endif /* Reader_h */
