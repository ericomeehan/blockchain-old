//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// put.h
//
// Eric Meehan
// 5/15/21
//
//

#ifndef CLNT_RT_put_h
#define CLNT_RT_put_h

#include "../../blockchain.h"

void BLOCKCHAIN_CLNT_RT_put(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* CLNT_RT_put_h */
