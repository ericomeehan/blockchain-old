//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// sql.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef SRV_RT_sql_h
#define SRV_RT_sql_h

#include "../../blockchain.h"
#include "../server.h"

void BLOCKCHAIN_SRV_RT_sql(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* SRV_RT_sql_h */
