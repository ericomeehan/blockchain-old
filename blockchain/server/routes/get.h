//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// get.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef SRV_RT_get_h
#define SRV_RT_get_h

#include "../../blockchain.h"
#include "../server.h"

void BLOCKCHAIN_SRV_RT_get(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* SRV_RT_get_h */
