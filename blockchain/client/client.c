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

#include "routes/get.h"
#include "routes/put.h"
#include "routes/sql.h"

#include "../utilities/logger.h"

static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size);
static void BLOCKCHAIN_CLNT_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
static void BLOCKCHAIN_CLNT_PRIVATE_register_default_routes(void);

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Initializing blockchain client...\n");
    BLOCKCHAIN_CLNT_PRIVATE_initialize(address, route, data_to_send, data_size);
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Blockchain client initialized\n");
    bool done = false;
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Making request\n");
    while (!done) {mg_mgr_poll(CLIENT, CLIENT_PULSE);}
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Connection closed\n");
    return done;
}



static bool BLOCKCHAIN_CLNT_PRIVATE_initialize(char *address, byte *route, byte *data_to_send, unsigned long data_size)
{
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Login required\n");
    BLOCKCHAIN_OBJ_Account user;
    if (!BLOCKCHAIN_OBJ_Account_login(&user))
    {
        BLOCKCHAIN_UTIL_logger(stderr, "BLOCKCHAIN ERROR: Failed to login\n");
        return false;
    }
    
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Preparing client variables...\n");
    CLIENT_ROUTES = dictionary_constructor(compare_string_keys);
    BLOCKCHAIN_CLNT_PRIVATE_register_default_routes();
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Client variables prepared\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Preparing client session...\n");
    BLOCKCHAIN_OBJ_LinkedBlock session = {NULL, NULL};
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Client session prepared\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Initializing Mongoose event manager...\n");
    mg_mgr_init(CLIENT);
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Mongoose initialized\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Connecting to server...\n");
    BLOCKCHAIN_CLNT_OBJ_request request = {&user, session, route, data_to_send, data_size};
    mg_connect(CLIENT, address, BLOCKCHAIN_CLNT_PRIVATE_event_handler, &request);
    BLOCKCHAIN_UTIL_logger(stdout, "BLOCKCHAIN CLIENT: Connection established\n");
    return true;
}

void BLOCKCHAIN_SRV_PRIVATE_register_default_routes(void)
{
    BLOCKCHAIN_CLNT_OBJ_Route get_route;
    get_route.route_function = BLOCKCHAIN_CLNT_RT_get;
    CLIENT_ROUTES.insert(&CLIENT_ROUTES, BLOCKCHAIN_CLNT_ROUTE_ADDRESSES, 1, &get_route, sizeof(get_route));
    
    BLOCKCHAIN_CLNT_OBJ_Route put_route;
    put_route.route_function = BLOCKCHAIN_CLNT_RT_put;
    CLIENT_ROUTES.insert(&CLIENT_ROUTES, BLOCKCHAIN_CLNT_ROUTE_ADDRESSES + 1, 1, &get_route, sizeof(get_route));
    
    BLOCKCHAIN_CLNT_OBJ_Route sql_route;
    sql_route.route_function = BLOCKCHAIN_CLNT_RT_sql;
    CLIENT_ROUTES.insert(&CLIENT_ROUTES, BLOCKCHAIN_CLNT_ROUTE_ADDRESSES + 2, 1, &get_route, sizeof(get_route));
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
