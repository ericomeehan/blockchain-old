//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// client.h
//
// Eric Meehan
// 5/15/21
//
//

#ifndef CLNT_client_h
#define CLNT_client_h

#include "../blockchain.h"

#include "../objects/account.h"
#include "../objects/block.h"
#include "../objects/session.h"

typedef struct BLOCKCHAIN_CLNT_OBJ_Route
{
    void (*route_function)(struct mg_connection *, int, void *, void *);
}
BLOCKCHAIN_CLNT_OBJ_Route;


typedef struct BLOCKCHAIN_CLNT_OBJ_request
{
    BLOCKCHAIN_OBJ_Account *user;
    BLOCKCHAIN_OBJ_LinkedBlock events;
    byte *route;
    byte *data;
    unsigned long data_size;
}
BLOCKCHAIN_CLNT_OBJ_request;

enum BLOCKCHAIN_CLNT_labels
{
    BLOCKCHAIN_CONNECTION_LABEL_STATUS,
    BLOCKCHAIN_CONNECTION_LABEL_ITERATION,
    BLOCKCHAIN_CONNECTION_LABEL_UUID
};

enum BLOCKCHAIN_CLNT_statuses
{
    BLOCKCHAIN_CLIENT_INTRODUCTION,
    BLOCKCHAIN_CLIENT_ROUTING,
    BLOCKCHAIN_CLIENT_EXCHANGE,
    BLOCKCHAIN_CLIENT_CLOSING
};

static struct Dictionary CLIENT_ROUTES;
static byte BLOCKCHAIN_CLNT_ROUTE_ADDRESSES[3] = {1, 2, 3};

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, byte *data_to_send, unsigned long data_size);

#endif /* CLNT_client_h */
