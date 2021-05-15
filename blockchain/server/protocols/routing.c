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
// 5/14/21
//
//

#include "routing.h"
#include "../../objects/Block.h"

void BLOCKCHAIN_SRV_PTCL_routing(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
            if (c->recv.len > 32)
            {
                status = BLOCKCHAIN_SERVER_CLOSING;
                response = 0;
                mg_send(c, &response, 1);
                break;
            }
            void * (*route)(void *) = routes.search(&routes, c->recv.buf, c->recv.len);
            mg_iobuf_delete(&c->recv, c->recv.len);
            if (!route)
            {
                status = BLOCKCHAIN_SERVER_CLOSING;
                response = 0;
                mg_send(c, &response, 1);
            }
            void *route_address = &c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE];
            route_address = route;
            status = BLOCKCHAIN_SERVER_EXCHANGE;
            response = 1;
            mg_send(c, &response, 1);
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
