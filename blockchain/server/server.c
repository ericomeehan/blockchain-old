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

#include "../utilities/logger.h"

volatile sig_atomic_t active = false;

static void BLOCKCHAIN_SRV_PRIVATE_register_default_routes(void);
static void BLOCKCHAIN_SRV_PRIVATE_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
static void BLOCKCHAIN_SRV_PRIVATE_terminate(int signum);

bool BLOCKCHAIN_SRV_initialize()
{
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing blockchain server...\n");
    BLOCKCHAIN_UTIL_logger(stdout, "Login required\n");
    if (!BLOCKCHAIN_OBJ_Account_login(&server_session.user))
    {
        BLOCKCHAIN_UTIL_logger(stderr, "BLOCKCHAIN ERROR: Failed to initialize server\n");
        return false;
    }
    
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing server varaiables...\n");
    routes = dictionary_constructor(compare_string_keys);
    BLOCKCHAIN_SRV_PRIVATE_register_default_routes();
    BLOCKCHAIN_UTIL_logger(stdout, "Server variables initialized\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing Mongoose event manager...\n");
    mg_mgr_init(&SERVER);
    BLOCKCHAIN_UTIL_logger(stdout, "Mongoose Initialized\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "Initializing SQLite database...\n");
    BLOCKCHAIN_DB_init();
    BLOCKCHAIN_UTIL_logger(stdout, "SQLite initialized\n");
    
    BLOCKCHAIN_UTIL_logger(stdout, "Mining session block...\n");
    byte hash[64] = {0};
    uuid_t binuuid;
    uuid_generate_random(binuuid);
    char uuid[37];
    uuid_unparse_upper(binuuid, uuid);
    BLOCKCHAIN_OBJ_Block_mine(&server_session.user, NULL, uuid, 37, hash);
    BLOCKCHAIN_OBJ_Block_load(server_session.whoami, hash);
    BLOCKCHAIN_UTIL_logger(stdout, "Session block mined\n");

    BLOCKCHAIN_UTIL_logger(stdout, "Server initialized\n");
    
    
    if (isatty(fileno(stderr)))
    {
        BLOCKCHAIN_UTIL_logger(stdout, "Redirecting stdout and stderr to logs\n");
        char path[256] = {0};
        strcat(path, LOG_PATH);
        strcat(path, "server_error.log");
        freopen(path, "a+", stderr);
        memset(path, 0, 256);
        strcat(path, LOG_PATH);
        strcat(path, "server_event.log");
        freopen(path, "a+", stdout);
    }
    
    
    return true;
}

void BLOCKCHAIN_SRV_launch(void)
{
    char listen_address[256] = {0};
    sprintf(listen_address, URL, PORT);
    
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = BLOCKCHAIN_SRV_PRIVATE_terminate;
    sigaction(SIGTERM, &action, NULL);
    
    mg_listen(&SERVER, listen_address, BLOCKCHAIN_SRV_PRIVATE_event_handler, &server_session);
    active = true;
    while (active)
    {
        mg_mgr_poll(&SERVER, SERVER_PULSE);
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
            c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS] = BLOCKCHAIN_SERVER_INTRODUCTION;
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
