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
// 5/15/21
//
//

#include "get.h"

void BLOCKCHAIN_CLNT_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    switch (ev)
    {
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
            break;
        }
        default:
        {
            break;
        }
    }
}