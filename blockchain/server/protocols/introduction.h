//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// introduction.h
//
// Eric Meehan
// 5/13/21
//
//

#ifndef SRV_PTCL_introduction_h
#define SRV_PTCL_introduction_h

#include "../../blockchain.h"
#include "../server.h"

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* SRV_PTCL_introduction_h */
