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
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_PTCL_close(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    char log[512] = {0};
    
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
            sprintf(log, CLIENT_LOGGING_FORMAT, request->whoami->data, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "CLOSED");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            break;
        }
        default:
        {
            break;
        }
    }
}
