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
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_exchange(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
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
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            void (*route)(struct mg_connection *, int, void *, void *) = routes.search(&routes, &c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE], strlen(&c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE]));
            
            route(c, ev, ev_data, fn_data);
            
            sprintf(log, BLOCKCHAIN_SRV_PTCL_exchange_log_format, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, &c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE], response);
            
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            
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
