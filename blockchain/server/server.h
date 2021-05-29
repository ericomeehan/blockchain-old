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

typedef struct Route
{
    void (*route_function)(struct mg_connection *, int, void *, void *);
}
BLOCKCHAIN_SRV_OBJ_Route;

enum BLOCKCHAIN_SRV_labels
{
    BLOCKCHAIN_CONNECTION_LABEL_STATUS,
    BLOCKCHAIN_CONNECTION_LABEL_SESSION,
    BLOCKCHAIN_CONNECTION_LABEL_ROUTE = 5,
    BLOCKCHAIN_CONNECTION_LABEL_UUID = 9
};

enum BLOCKCHAIN_SRV_statuses
{
    BLOCKCHAIN_SERVER_INTRODUCTION,
    BLOCKCHAIN_SERVER_ROUTING,
    BLOCKCHAIN_SERVER_EXCHANGE,
    BLOCKCHAIN_SERVER_CLOSING
};

static BLOCKCHAIN_OBJ_Session SERVER_SESSION;
static struct Dictionary SERVER_ROUTES;
static byte BLOCKCHAIN_SERVER_ROUTE_ADDRESSES[3] = {1, 2, 3};

bool BLOCKCHAIN_SRV_initialize(void);
void BLOCKCHAIN_SRV_launch(void);

#endif /* SRV_server_h */
