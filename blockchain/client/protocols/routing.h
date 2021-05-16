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
// 5/15/21
//
//

#ifndef CLNT_PTCL_routing_h
#define CLNT_PTCL_routing_h

#include "../../blockchain.h"
#include "../client.h"

void BLOCKCHAIN_CLNT_PTCL_routing(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* CLNT_PTCL_routing_h */
