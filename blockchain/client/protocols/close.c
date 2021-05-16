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
// 5/15/21
//
//

#include "close.h"

void BLOCKCHAIN_CLNT_PTCL_close(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    switch (ev)
    {
        case MG_EV_POLL:
        {
            break;
        }
        case MG_EV_RESOLVE:
        {
            break;
        }
        case MG_EV_CONNECT:
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
