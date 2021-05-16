//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// client.c
//
// Eric Meehan
// 5/15/21
//
//

#include "client.h"

#include "protocols/exchange.h"
#include "protocols/introduction.h"
#include "protocols/routing.h"
#include "protocols/close.h"

static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size);
static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_CLNT_PRIVATE_initialize(address, route, data_to_send, data_size);
    bool done = false;
    while (!done) {mg_mgr_poll(CLIENT, CLIENT_PULSE);}
    return done;
}



static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_OBJ_Account user;
    if (!BLOCKCHAIN_OBJ_Account_login(&user))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to login\n");
        return false;
    }
    BLOCKCHAIN_OBJ_Block *whoami = NULL;
    // mine whoami block
    mg_mgr_init(CLIENT);
    BLOCKCHAIN_CLNT_OBJ_request request = {whoami, route, data_to_send, data_size};
    mg_connect(CLIENT, address, BLOCKCHAIN_CLNT_PRIVATE_event_handler, &request);
    return true;
}

static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    switch (c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS]) {
        case BLOCKCHAIN_CLIENT_INTRODUCTION:
        {
            break;
        }
        case BLOCKCHAIN_CLIENT_ROUTING:
        {
            break;
        }
        case BLOCKCHAIN_CLIENT_EXCHANGE:
        {
            break;
        }
        case BLOCKCHAIN_CLIENT_CLOSING:
        {
            break;
        }
        default:
            break;
    }
}
