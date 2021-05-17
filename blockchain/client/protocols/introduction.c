//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// introduction.c
//
// Eric Meehan
// 5/15/21
//
//

#include "introduction.h"
#include "../../database/database.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    int stage = c->label[BLOCKCHAIN_CONNECTION_LABEL_ITERATION];
    byte response = 0;
    
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_POLL:
        {
            break;
        }
        case MG_EV_RESOLVE:
        {
            sprintf(log, CLIENT_LOGGING_FORMAT, "N/A", c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "RESOLVED");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            break;
        }
        case MG_EV_CONNECT:
        {
            sprintf(log, CLIENT_LOGGING_FORMAT, "N/A", c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "CONNECTED");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            break;
        }
        case MG_EV_READ:
        {
            switch (stage) {
                case 0:
                {
                    status = ((response = BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)c->recv.buf)) ? BLOCKCHAIN_CLIENT_ROUTING:BLOCKCHAIN_CLIENT_CLOSING);
                    
                    byte hash[64] = {0};
                    BLOCKCHAIN_OBJ_Block_mine(request->user, (BLOCKCHAIN_OBJ_Block *)c->recv.buf, c->recv.buf + sizeof(BLOCKCHAIN_OBJ_BlockHeaders), 16, hash);
                    
                    BLOCKCHAIN_OBJ_Block *whoami = NULL;
                    BLOCKCHAIN_OBJ_Block_load(whoami, hash);
                    
                    mg_send(c, whoami, whoami->headers.size);
                    
                    sprintf(log, CLIENT_LOGGING_FORMAT, whoami->data, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "SESSION BLOCK SENT");
                    BLOCKCHAIN_UTIL_logger(stdout, log);
                    break;
                }
                case 1:
                {
                    status = (c->recv.buf[0] ? BLOCKCHAIN_CLIENT_ROUTING:BLOCKCHAIN_CLIENT_CLOSING);
                    
                    sprintf(log, CLIENT_LOGGING_FORMAT, request->whoami->data, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "SERVER SESSION RESPONSE");
                    BLOCKCHAIN_UTIL_logger(stdout, log);

                    break;
                }
                default:
                    break;
            }
            
            break;
        }
        case MG_EV_WRITE:
        {
            stage += 1;
            break;
        }
        case MG_EV_CLOSE:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}
