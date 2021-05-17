//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// sql.c
//
// Eric Meehan
// 5/15/21
//
//

#include "sql.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_RT_sql(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
            BLOCKCHAIN_UTIL_logger(stdout, (char *)c->recv.buf);
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
