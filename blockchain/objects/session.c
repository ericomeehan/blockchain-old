//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// OBJ_session.c
//
// Eric Meehan
// 5/15/21
//
//

#include "session.h"

void BLOCKCHAIN_OBJ_LinkedBlock_next(BLOCKCHAIN_OBJ_LinkedBlock *linked_block)
{
    free(linked_block->previous);
    linked_block->previous = linked_block->block;
    linked_block->block = NULL;
}
