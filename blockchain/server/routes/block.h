//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// block.h
//
// Eric Meehan
// 5/12/21
//
//

#ifndef block_h
#define block_h

#include "../../blockchain.h"

static void block_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* block_h */
