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
// 5/15/21
//
//

#ifndef CLNT_RT_sql_h
#define CLNT_RT_sql_h

#include "../../blockchain.h"

void BLOCKCHAIN_CLNT_RT_sql(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* CLNT_RT_sql_h */
