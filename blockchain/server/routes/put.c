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

void BLOCKCHAIN_SRV_RT_put(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
            if (c->recv.len == sizeof(BLOCKCHAIN_OBJ_BlockHeaders))
            {
                BLOCKCHAIN_OBJ_BlockHeaders headers;
                memcpy(&headers, c->recv.buf, c->recv.len);
                mg_iobuf_free(&c->recv);
                
                bool result = headers.size <= MAX_BLOCK_SIZE;
                status = BLOCKCHAIN_SERVER_CLOSING;
                mg_send(c, &result, sizeof(bool));
            }
            else
            {
                BLOCKCHAIN_OBJ_Block *block = malloc(c->recv.len);
                memcpy(block, c->recv.buf, c->recv.len);
                mg_iobuf_free(&c->recv);
                bool result = BLOCKCHAIN_OBJ_Block_validate(block);
                if (result)
                {
                    BLOCKCHAIN_OBJ_Block_save(block);
                }
                status = BLOCKCHAIN_SERVER_CLOSING;
                free(block);
                mg_send(c, &result, sizeof(bool));
            }
            
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
