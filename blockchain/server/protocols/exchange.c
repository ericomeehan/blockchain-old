//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// exchange.c
//
// Eric Meehan
// 5/14/21
//
//

#include "exchange.h"

#include "../../objects/Block.h"

void BLOCKCHAIN_SRV_PTCL_exchange(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_SRV_OBJ_Session *session = (BLOCKCHAIN_SRV_OBJ_Session *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_DATA];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_SRV_OBJ_ServerData *data = fn_data;
    byte response;
    switch (ev)
    {
        case MG_EV_ACCEPT:
        {
            status = BLOCKCHAIN_SERVER_INTRODUCTION;
            break;
        }
        case MG_EV_READ:
        {
            void (*route)(struct mg_connection *, int, void *, void *) = (void (*)(struct mg_connection *, int, void *, void *))&c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE];
            route(c, ev, ev_data, fn_data);
            break;
        }
        case MG_EV_WRITE:
        {
            // Loggers...
            break;
        }
        case MG_EV_CLOSE:
        {
            status = BLOCKCHAIN_SERVER_CLOSING;
            // Loggers...
            break;
        }
        default:
        {
            break;
        }
    }
}
