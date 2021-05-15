//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// close.c
//
// Eric Meehan
// 5/14/21
//
//

#include "close.h"
#include "../../objects/Block.h"

void BLOCKCHAIN_SRV_PTCL_close(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
