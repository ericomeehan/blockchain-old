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

#ifndef client_h
#define client_h

#include "../blockchain.h"

#include "../objects/Account.h"
#include "../objects/Block.h"

typedef struct BLOCKCHAIN_CLNT_OBJ_request
{
    BLOCKCHAIN_OBJ_Block *whoami;
    byte *route;
    unsigned long route_size;
    byte *data;
    unsigned long data_size;
}
BLOCKCHAIN_CLNT_OBJ_request;

enum BLOCKCHAIN_CLNT_statuses
{
    BLOCKCHAIN_SERVER_INTRODUCTION,
    BLOCKCHAIN_SERVER_ROUTING,
    BLOCKCHAIN_SERVER_EXCHANGE,
    BLOCKCHAIN_SERVER_CLOSING
};

bool BLOCKCHAIN_CLNT_request(char *address, byte *route, unsigned long route_size, byte *data_to_send, unsigned long data_size);

#endif /* client_h */
