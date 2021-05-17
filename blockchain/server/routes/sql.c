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
#include "../../database/database.h"
#include "../../utilities/logger.h"

static int BLOCKCHAIN_SRV_RT_PRIVATE_sql_callback(void *, int, char **, char **);

void BLOCKCHAIN_SRV_RT_sql(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_OBJ_Session *session = (BLOCKCHAIN_OBJ_Session *)fn_data;
    byte *uuid = (byte *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte response = 0;
    
    char log[512] = {0};
    
    switch (ev)
    {
        case MG_EV_ACCEPT:
        {
            break;
        }
        case MG_EV_READ:
        {
            BLOCKCHAIN_OBJ_LinkedBlock_next(&session->events);
            
            response = BLOCKCHAIN_DB_query((char *)c->recv.buf, BLOCKCHAIN_SRV_RT_PRIVATE_sql_callback, c);
            
            mg_send(c, &response, 1);
            
            sprintf(log, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "sql", response, (char *)c->recv.buf);
            BLOCKCHAIN_UTIL_logger(stdout, log);
            
            byte hash[64] = {0};
            BLOCKCHAIN_OBJ_Block_mine(&session->user, session->events.previous, log, strlen(log) + 1, hash);
            BLOCKCHAIN_OBJ_Block_load(session->events.block, hash);
            
            break;
        }
        case MG_EV_WRITE:
        {
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

int BLOCKCHAIN_SRV_RT_PRIVATE_sql_callback(void *dt, int num_columns, char **column_text, char **columns)
{
    char row[4096] = {0};
    for (int i = 0; i < num_columns; i++)
    {
        strcat(row, column_text[i]);
        if (i < num_columns - 1) { strcat(row, ", "); }
        else { strcat(row, "\n"); }
    }
    mg_send(dt, row, strlen(row) + 1);
    return EXIT_SUCCESS;
}
