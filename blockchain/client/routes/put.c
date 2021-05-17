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
// 5/15/21
//
//

#include "put.h"

void BLOCKCHAIN_CLNT_RT_put(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    int stage = c->label[BLOCKCHAIN_CONNECTION_LABEL_ITERATION];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    switch (ev)
    {
        case MG_EV_POLL:
        {
            if (stage == 0) {mg_send(c, request->data, request->data_size);}
        }
        case MG_EV_ACCEPT:
        {
            break;
        }
        case MG_EV_READ:
        {
            switch (stage) {
                case 1:
                {
                    mg_send(c, &((BLOCKCHAIN_OBJ_Block *)request->data)->headers, sizeof(BLOCKCHAIN_OBJ_BlockHeaders));
                    break;
                }
                case 2:
                {
                    mg_send(c, request->data, request->data_size);
                    break;
                }
                default:
                    break;
            }
            status = BLOCKCHAIN_CLIENT_CLOSING;
            break;
        }
        case MG_EV_WRITE:
        {
            stage += 1;
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
