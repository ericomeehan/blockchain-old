//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// introduction.c
//
// Eric Meehan
// 5/13/21
//
//

#include "introduction.h"

#include "../../database/database.h"
#include "../../database/setup/commands.h"

#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
            mg_send(c, session->whoami, session->whoami->headers.size);
            sprintf(log, BLOCKCHAIN_SRV_PTCL_introduction_log_format_ev_accept_format, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status);
            BLOCKCHAIN_UTIL_logger(stdout, log);
            break;
        }
        case MG_EV_READ:
        {
            status = ((response = BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)&c->recv.buf)) ? BLOCKCHAIN_SERVER_ROUTING:BLOCKCHAIN_SERVER_CLOSING);
            if (response)
            {
                BLOCKCHAIN_DB_insert_block((BLOCKCHAIN_OBJ_Block *)&c->recv.buf);
                byte hash[64] = {0};
                BLOCKCHAIN_OBJ_Block_hash((BLOCKCHAIN_OBJ_Block *)&c->recv.buf, hash);
                BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            }
            mg_iobuf_delete(&c->recv, c->recv.len);
            mg_send(c, &response, 1);
            
            sprintf(log, BLOCKCHAIN_SRV_PTCL_introduction_log_format_ev_read_format, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, response);
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
