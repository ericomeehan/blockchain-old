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
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_close(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte *uuid = (byte *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response = 0;
    
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_POLL:
        {
            break;
        }
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
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "CLOSE", response, "CLOSING");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);            
            free(session->events.previous);
            break;
        }
        default:
        {
            break;
        }
    }
}
