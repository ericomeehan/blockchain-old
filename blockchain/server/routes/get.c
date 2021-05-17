//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// get.c
//
// Eric Meehan
// 5/14/21
//
//

#include "get.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte *uuid = (byte *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response;
    
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_ACCEPT:
        {
            break;
        }
        case MG_EV_READ:
        {
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            BLOCKCHAIN_OBJ_Block *block = NULL;
            response = BLOCKCHAIN_OBJ_Block_load(block, c->recv.buf);
            mg_send(c, (response ? (void *)block:(void *)&response), (response ? block->headers.size:1));
            status = BLOCKCHAIN_SERVER_CLOSING;
            
            BIGNUM *value = BN_new();
            BN_bin2bn(c->recv.buf, (int)c->recv.len, value);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "get", response, BN_bn2hex(value));
            
            if (block) { free(block); }
            BN_free(value);
            mg_iobuf_delete(&c->recv, c->recv.len);
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
