//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// sql.c
//
// Eric Meehan
// 5/14/21
//
//

#include "sql.h"

void BLOCKCHAIN_SRV_RT_sql(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{ 
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response;
}
