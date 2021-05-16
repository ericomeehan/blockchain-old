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
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_routing(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response;
    
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
            response = (routes.search(&routes, c->recv.buf, c->recv.len) ? 1:0);
            status = (response ? BLOCKCHAIN_SERVER_EXCHANGE:BLOCKCHAIN_SERVER_CLOSING);
            
            if (response) {strcpy(&c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE], (const char *)&c->recv);}
            
            sprintf(log, BLOCKCHAIN_SRV_PTCL_routing_log_format, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, c->recv.buf, response);
            mg_iobuf_delete(&c->recv, c->recv.len);
            mg_send(c, &response, 1);
            
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);

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
