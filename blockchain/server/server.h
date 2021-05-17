//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// SRV_server.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef SRV_server_h
#define SRV_server_h

#include "../blockchain.h"

#include "../objects/account.h"
#include "../objects/block.h"
#include "../objects/session.h"

typedef struct ThreadPool ThreadPool;
typedef struct ThreadJob ThreadJob;

typedef struct Route
{
    void (*route_function)(struct mg_connection *, int, void *, void *);
}
BLOCKCHAIN_SRV_OBJ_Route;

static byte server_routes[3] = {1, 2, 3};

enum BLOCKCHAIN_SRV_labels
{
    BLOCKCHAIN_CONNECTION_LABEL_STATUS,
    BLOCKCHAIN_CONNECTION_LABEL_ROUTE,
    BLOCKCHAIN_CONNECTION_LABEL_UUID = sizeof(void *) + 1,
};

enum BLOCKCHAIN_SRV_statuses
{
    BLOCKCHAIN_SERVER_INTRODUCTION,
    BLOCKCHAIN_SERVER_ROUTING,
    BLOCKCHAIN_SERVER_EXCHANGE,
    BLOCKCHAIN_SERVER_CLOSING
};

static BLOCKCHAIN_OBJ_Session server_session;
static struct Dictionary routes;

bool BLOCKCHAIN_SRV_initialize(void);
void BLOCKCHAIN_SRV_launch(void);

#endif /* SRV_server_h */
