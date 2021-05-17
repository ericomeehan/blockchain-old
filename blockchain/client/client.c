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

#include "../utilities/logger.h"

static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size);
static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_CLNT_PRIVATE_initialize(address, route, data_to_send, data_size);
    bool done = false;
    BLOCKCHAIN_UTIL_logger(stdout, "Making request\n");
    while (!done) {mg_mgr_poll(CLIENT, CLIENT_PULSE);}
    BLOCKCHAIN_UTIL_logger(stdout, "Connection closed\n");
    return done;
}



static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing blockchain client\n");
    BLOCKCHAIN_UTIL_logger(stdout, "Login required\n");
    BLOCKCHAIN_OBJ_Account user;
    if (!BLOCKCHAIN_OBJ_Account_login(&user))
    {
        BLOCKCHAIN_UTIL_logger(stderr, "BLOCKCHAIN ERROR: Failed to login\n");
        return false;
    }
    
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing Mongoose event manager\n");
    mg_mgr_init(CLIENT);
    BLOCKCHAIN_UTIL_logger(stdout, "Mongoose initialized\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "Mining session block\n");
    BLOCKCHAIN_OBJ_Block *whoami = NULL;
    // mine
    BLOCKCHAIN_UTIL_logger(stdout, "Session block mined\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "Connecting to server\n");
    BLOCKCHAIN_CLNT_OBJ_request request = {&user, whoami, route, data_to_send, data_size};
    mg_connect(CLIENT, address, BLOCKCHAIN_CLNT_PRIVATE_event_handler, &request);
    BLOCKCHAIN_UTIL_logger(stdout, "Connection established\n");
    return true;
}

void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    switch (c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS]) {
        case BLOCKCHAIN_CLIENT_INTRODUCTION:
        {
            BLOCKCHAIN_CLNT_PTCL_introduction(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_CLIENT_ROUTING:
        {
            BLOCKCHAIN_CLNT_PTCL_routing(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_CLIENT_EXCHANGE:
        {
            BLOCKCHAIN_CLNT_PTCL_exchange(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_CLIENT_CLOSING:
        {
            BLOCKCHAIN_CLNT_PTCL_close(c, ev, ev_data, fn_data);
            break;
        }
        default:
            c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS] = BLOCKCHAIN_CLIENT_INTRODUCTION;
            break;
    }
}
