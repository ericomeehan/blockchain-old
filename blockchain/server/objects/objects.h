//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// SRV_OBJ_objects.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef SRV_OBJ_objects_h
#define SRV_OBJ_objects_h

#include "../../blockchain.h"
#include "../../objects/Block.h"

typedef struct LinkedBlock
{
    BLOCKCHAIN_OBJ_Block *previous;
    BLOCKCHAIN_OBJ_Block *data;
}
BLOCKCHAIN_SRV_OBJ_LinkedBlock;

typedef struct Session
{
    int blocking_socket;
    int non_blocking_socket;
    BLOCKCHAIN_SRV_OBJ_LinkedBlock *event;
}
BLOCKCHAIN_SRV_OBJ_Session;

typedef struct Route
{
    void * (*route_function)(void *arg);
}
BLOCKCHAIN_SRV_OBJ_Route;

typedef struct ServerData
{
    BLOCKCHAIN_OBJ_Account user;
    BLOCKCHAIN_OBJ_Block *whoami;
}
BLOCKCHAIN_SRV_OBJ_ServerData;

#endif /* SRV_OBJ_objects_h */
