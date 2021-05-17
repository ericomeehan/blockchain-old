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
// 5/13/21
//
//

#include "introduction.h"
#include "../../database/database.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte *uuid = (byte *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    byte response = 0;
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_POLL:
        {
            break;
        }
        case MG_EV_ACCEPT:
        {
            BLOCKCHAIN_UTIL_logger(stdout, "Mining session block...\n");
            byte hash[64] = {0};
            uuid_generate(uuid);
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->whoami, uuid, 16, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            BLOCKCHAIN_UTIL_logger(stdout, "Session block mined\n");
            
            mg_send(c, session->events.block, session->events.block->headers.size);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "INTRODUCTION", response, "SESSION BLOCK SENT");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            
            break;
        }
        case MG_EV_READ:
        {
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            
            status = ((response = BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)&c->recv.buf)) ? BLOCKCHAIN_SERVER_ROUTING:BLOCKCHAIN_SERVER_CLOSING);
            
            if (response)
            {
                BLOCKCHAIN_DB_insert_block((BLOCKCHAIN_OBJ_Block *)&c->recv.buf);
                byte hash[64] = {0};
                BLOCKCHAIN_OBJ_Block_hash((BLOCKCHAIN_OBJ_Block *)&c->recv.buf, hash);
                BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
                BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            }
            
            mg_iobuf_delete(&c->recv, c->recv.len);
            mg_send(c, &response, 1);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "INTRODUCTION", response, "CLIENT CONFIRMATION RECEIVED");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log), hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            break;
        }
        case MG_EV_WRITE:
        {
            break;
        }
        case MG_EV_CLOSE:
        {
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            
            status = BLOCKCHAIN_SERVER_CLOSING;
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "INTRODUCTION", response, "CLOSING");
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            free(session->events.previous);
            break;
        }
        default:
        {
            break;
        }
    }
}
