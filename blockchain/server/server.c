//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// server.c
//
// Eric Meehan
// 5/12/21
//
//

#include "server.h"

#include "routes/block.h"

static struct ThreadPool thread_pool;
static struct Dictionary routes;

struct ThreadArg
{
    struct mg_connection *c;
    int ev;
    void *ev_data;
    void *fn_data;
}

static void event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

int initialize_server()
{
    mg_mgr_init(SERVER);
    thread_pool = thread_pool_constructor(256);
    routes = dictionary_constructor(compare_string_keys);
}

static void event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    struct ThreadArg arg = {c, ev, ev_data, fn_data};
    struct ThreadJob thread_job = thread_job_constructor(<#void *(*job)(void *)#>, <#void *arg#>)
}
