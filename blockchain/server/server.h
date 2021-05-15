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

#include "../objects/Account.h"
#include "../objects/Block.h"

#include "objects/objects.h"

typedef struct ThreadPool ThreadPool;
typedef struct ThreadJob ThreadJob;

static byte server_routes[3] = {1, 2, 3};

enum labels
{
    BLOCKCHAIN_CONNECTION_LABEL_STATUS,
    BLOCKCHAIN_CONNECTION_LABEL_DATA,
    BLOCKCHAIN_CONNECTION_LABEL_ROUTE = 9,
};

enum statuses
{
    BLOCKCHAIN_SERVER_INTRODUCTION,
    BLOCKCHAIN_SERVER_ROUTING,
    BLOCKCHAIN_SERVER_EXCHANGE,
    BLOCKCHAIN_SERVER_CLOSING
};

static ThreadPool thread_pool;
static struct Dictionary routes;
static BLOCKCHAIN_SRV_OBJ_ServerData data;

bool BLOCKCHAIN_SRV_initialize(void);
void BLOCKCHAIN_SRV_launch(void);

#endif /* SRV_server_h */
