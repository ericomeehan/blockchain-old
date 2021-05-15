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
#include "../../objects/Block.h"
#include "../../database/database.h"
#include "../../database/setup/commands.h"

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    BLOCKCHAIN_SRV_OBJ_Session *session = (BLOCKCHAIN_SRV_OBJ_Session *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_DATA];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    BLOCKCHAIN_SRV_OBJ_ServerData *data = fn_data;
    byte response;
    switch (ev)
    {
        case MG_EV_ACCEPT:
        {
            if (!session)
            {
                session = malloc(sizeof(BLOCKCHAIN_SRV_OBJ_Session));
                session->blocking_socket = -1;
                session->non_blocking_socket = -1;
            }
            status = BLOCKCHAIN_SERVER_INTRODUCTION;
            mg_send(c, data->whoami, data->whoami->headers.size);
            // Session logging...
            break;
        }
        case MG_EV_READ:
        {
            BLOCKCHAIN_OBJ_Block *whoareyou = (BLOCKCHAIN_OBJ_Block *)c->recv.buf;
            if (!(response = BLOCKCHAIN_OBJ_Block_validate(whoareyou)))
            {
                status = BLOCKCHAIN_SERVER_CLOSING;
            }
            else
            {
                status = BLOCKCHAIN_SERVER_ROUTING;
                // Insert into database
                BLOCKCHAIN_DB_insert_block(whoareyou);
            }
            mg_iobuf_delete(&c->recv, c->recv.len);
            mg_send(c, &response, 1);
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
