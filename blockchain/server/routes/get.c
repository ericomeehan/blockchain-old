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
// 5/14/21
//
//

#include "get.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];

    byte response = 0;
    
    BLOCKCHAIN_OBJ_Block *block = NULL;
    response = BLOCKCHAIN_OBJ_Block_load(block, c->recv.buf);
    mg_iobuf_delete(&c->recv, c->recv.len);
    mg_send(c, (response ? (void *)block : &response), (response ? block->headers.size : 1));
    status = BLOCKCHAIN_SERVER_CLOSING;
    
}
