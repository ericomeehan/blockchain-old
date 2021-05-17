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
// 5/15/21
//
//

#include "exchange.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_PTCL_exchange(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_POLL:
        {
            mg_send(c, request->route, strlen((const char *)request->route) + 1);
            sprintf(log, CLIENT_LOGGING_FORMAT, request->whoami->data, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "REQUEST SENT TO ROUTE");
            BLOCKCHAIN_UTIL_logger(stdout, log);
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
            sprintf(log, CLIENT_LOGGING_FORMAT, request->whoami->data, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "SERVER RESPONSE");
            BLOCKCHAIN_UTIL_logger(stdout, log);
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
