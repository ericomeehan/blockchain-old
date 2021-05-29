//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// get.c
//
// Eric Meehan
// 5/15/21
//
//

#include "get.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    if ((BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *) c->recv.buf)))
    {BLOCKCHAIN_OBJ_Block_save((BLOCKCHAIN_OBJ_Block *)c->recv.buf);}
}
