//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// routing.c
//
// Eric Meehan
// 5/15/21
//
//

#include "routing.h"

void BLOCKCHAIN_CLNT_PTCL_routing(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    int *iteration = (int *)c->label;
    switch (ev)
    {
        case MG_EV_POLL:
        {
            if (!*iteration)
            {
                mg_send(c, request->route, strlen((const char *)request->route) + 1);
                *iteration += 1;
            }
            
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
            status = (c->recv.buf[0] ? BLOCKCHAIN_CLIENT_EXCHANGE:BLOCKCHAIN_CLIENT_CLOSING);
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
