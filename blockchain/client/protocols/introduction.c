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
// 5/15/21
//
//

#include "introduction.h"
#include "../../database/database.h"

void BLOCKCHAIN_CLNT_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    int *iteration = (int *)c->label;
    byte response = 0;
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
            switch (*iteration) {
                case 0:
                {
                    status = (BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)c->recv.buf) ? BLOCKCHAIN_CLIENT_ROUTING:BLOCKCHAIN_CLIENT_CLOSING);
                    byte hash[64] = {0};
                    BLOCKCHAIN_OBJ_Block_mine(<#BLOCKCHAIN_OBJ_Account *user#>, c->recv.buf, <#void *data#>, <#unsigned long size#>, hash);
                    BLOCKCHAIN_OBJ_Block *whoami = NULL;
                    BLOCKCHAIN_OBJ_Block_load(whoami, hash);
                    mg_send(c, whoami, whoami->headers.size);
                    *iteration += 1;
                    break;
                }
                case 1:
                {
                    status = (c->recv.buf[0] ? BLOCKCHAIN_CLIENT_ROUTING:BLOCKCHAIN_CLIENT_CLOSING);
                    *iteration = 0;
                    break;
                }
                default:
                    break;
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
