//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// put.c
//
// Eric Meehan
// 5/14/21
//
//

#include "put.h"
#include "../../database/database.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_RT_put(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte *uuid = (byte *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response = 0;
    
    char *hex = NULL;
    
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
            if (c->recv.len == sizeof(BLOCKCHAIN_OBJ_BlockHeaders))
            {
                BLOCKCHAIN_OBJ_BlockHeaders headers;
                memcpy(&headers, c->recv.buf, c->recv.len);
                mg_iobuf_free(&c->recv);
                
                response = headers.size <= MAX_BLOCK_SIZE;
                status = BLOCKCHAIN_SERVER_CLOSING;
            }
            else
            {
                BLOCKCHAIN_OBJ_Block *block = malloc(c->recv.len);
                memcpy(block, c->recv.buf, c->recv.len);
                mg_iobuf_free(&c->recv);
                if ((response = BLOCKCHAIN_OBJ_Block_validate(block)))
                {
                    byte hash[64] = {0};
                    BLOCKCHAIN_OBJ_Block_hash(block, hash);
                    BIGNUM *value = BN_new();
                    BN_bin2bn(hash, 64, value);
                    hex = BN_bn2hex(value);
                    BLOCKCHAIN_OBJ_Block_save(block);
                }
                status = BLOCKCHAIN_SERVER_CLOSING;
                free(block);
            }
            
            mg_send(c, &response, 1);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "put", response, (response ? hex:"BLOCK REJECTED"));
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
