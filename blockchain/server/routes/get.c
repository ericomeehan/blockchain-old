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

void BLOCKCHAIN_SRV_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_SRV_OBJ_Session *session = (BLOCKCHAIN_SRV_OBJ_Session *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_DATA];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_SRV_OBJ_ServerData *data = fn_data;
    byte response;
    switch (ev)
    {
        case MG_EV_ACCEPT:
        {
            break;
        }
        case MG_EV_READ:
        {
            BLOCKCHAIN_OBJ_Block *block = NULL;
            response = BLOCKCHAIN_OBJ_Block_load(block, c->recv.buf);
            mg_send(c, (response ? (void *)block:(void *)&response), (response ? block->headers.size:1));
            status = BLOCKCHAIN_SERVER_CLOSING;
            mg_iobuf_free(&c->recv);
            if (block) { free(block); }
            break;
        }
        case MG_EV_WRITE:
        {
            break;
        }
        case MG_EV_CLOSE:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}
