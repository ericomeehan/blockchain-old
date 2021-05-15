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

static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, unsigned long route_size, byte *data_to_send, unsigned long data_size);
static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, unsigned long route_size, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_CLNT_PRIVATE_initialize(address, route, route_size, data_to_send, data_size);
    bool done = false;
    while (!done) {mg_mgr_poll(CLIENT, CLIENT_PULSE);}
    return done;
}



static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, unsigned long route_size, byte *data_to_send, unsigned long data_size)
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
    BLOCKCHAIN_CLNT_OBJ_request request = {whoami, route, route_size, data_to_send, data_size};
    mg_connect(CLIENT, address, BLOCKCHAIN_CLNT_PRIVATE_event_handler, &request);
    return true;
}

static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    switch (ev) {
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
            break;
    }
}
