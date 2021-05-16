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

const char *BLOCKCHAIN_SRV_PTCL_introduction_log_format_ev_accept_format = "{\"client\": {\"ip\": %lu, \"ip6\": %lu, \"is_ip6\": %d, \"port\": %lu}, \"status\": %d}\n";
const char *BLOCKCHAIN_SRV_PTCL_introduction_log_format_ev_read_format = "{\"client\": {\"ip\": %lu, \"ip6\": %lu, \"is_ip6\": %d, \"port\": %lu}, \"status\": %d, \"response\": %d}\n";

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

#endif /* SRV_PTCL_introduction_h */
