//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// SRV_server.c
//
// Eric Meehan
// 5/14/21
//
//

#include "server.h"

#include "../database/database.h"

#include "protocols/introduction.h"
#include "protocols/routing.h"
#include "protocols/exchange.h"
#include "protocols/close.h"

#include "routes/get.h"
#include "routes/put.h"
#include "routes/sql.h"

volatile sig_atomic_t active = false;

static void event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
static void terminate(int signum);
static void register_default_routes(void);

bool BLOCKCHAIN_SRV_initialize()
{
    if (!BLOCKCHAIN_OBJ_Account_login(&data.user))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to initialize server\n");
        return false;
    }
    
    mg_mgr_init(SERVER);
    thread_pool = thread_pool_constructor(64);
    routes = dictionary_constructor(compare_string_keys);
    
    register_default_routes();
    
    // Mine whoami block
    
    return true;
}

void BLOCKCHAIN_SRV_launch(void)
{
    char listen_address[256] = {0};
    sprintf(listen_address, URL, PORT);
    
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGTERM, &action, NULL);
    
    mg_listen(SERVER, listen_address, event_handler, &data);
    active = true;
    while (active)
    {
        mg_mgr_poll(SERVER, SERVER_PULSE);
    }
}


static void event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    switch (c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS])
    {
        case BLOCKCHAIN_SERVER_INTRODUCTION:
        {
            // Client and server create introduction blocks.
            BLOCKCHAIN_SRV_PTCL_introduction(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_SERVER_ROUTING:
        {
            // The client specifies a specific route for communications.
            BLOCKCHAIN_SRV_PTCL_routing(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_SERVER_EXCHANGE:
        {
            // The exchange occurs over the specified route.
            BLOCKCHAIN_SRV_PTCL_exchange(c, ev, ev_data, fn_data);
            break;
        }
        case BLOCKCHAIN_SERVER_CLOSING:
        {
            // The connection ends.
            BLOCKCHAIN_SRV_PTCL_close(c, ev, ev_data, fn_data);
            break;
        }
        default:
        {
            break;
        }
    }
}

void terminate(int signum)
{
    active = false;
}

void register_default_routes(void)
{
    BLOCKCHAIN_SRV_OBJ_Route get_route;
    get_route.route_function = BLOCKCHAIN_SRV_RT_get;
    routes.insert(&routes, server_routes, 1, &get_route, sizeof(get_route));
    
    BLOCKCHAIN_SRV_OBJ_Route put_route;
    put_route.route_function = BLOCKCHAIN_SRV_RT_put;
    routes.insert(&routes, server_routes + 1, 1, &get_route, sizeof(get_route));
    
    BLOCKCHAIN_SRV_OBJ_Route sql_route;
    sql_route.route_function = BLOCKCHAIN_SRV_RT_sql;
    routes.insert(&routes, server_routes + 2, 1, &get_route, sizeof(get_route));
}
