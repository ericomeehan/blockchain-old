//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// routing.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef SRV_PTCL_routing_h
#define SRV_PTCL_routing_h

#include "../../blockchain.h"
#include "../server.h"

void BLOCKCHAIN_SRV_PTCL_routing(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* SRV_PTCL_routing_h */
