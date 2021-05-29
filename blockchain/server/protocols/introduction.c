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
#include "../../utilities/uuid.h"

void BLOCKCHAIN_SRV_PTCL_introduction(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    // Each stage of the protocol utilizes the connection label to store a pointer to the client session, a status label, and the session uuid
    BLOCKCHAIN_OBJ_Session *client_session = (BLOCKCHAIN_OBJ_Session *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_SESSION];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    char *uuid = &c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    
    byte response = 0;
    
    switch (ev)
    {
        // An MG_EV_ERROR event occurs when there is a problem and the client should be closed
        case MG_EV_ERROR:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        // An MG_EV_POLL event occurs by default with each iteration of the server's infinite loop.
        case MG_EV_POLL:
            break;
        // An MG_EV_RESOLVE event occurs when the resolution of a host's domain to an ip
        case MG_EV_RESOLVE:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        // An MG_EV_CONNECT event occurs when a host machine has been connected to
        case MG_EV_CONNECT:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        // An MG_EV_ACCPET event represents a new client connection
        // This is where the session will be initialized
        case MG_EV_ACCEPT:
        {
            // Allocate memory for the session
            // NOTE: THIS NEEDS TO BE FREED IN THE CLOSE PROTOCOL
            client_session = malloc(sizeof(BLOCKCHAIN_OBJ_Session));
            // Generate a UUID for this client's session and mine it into a block
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_gen_uuid(uuid);
            BLOCKCHAIN_OBJ_Block_mine(&SERVER_SESSION.user, SERVER_SESSION.block, uuid, strlen(uuid) + 1, digest);
            // Load the client session block into the client_session struct
            BLOCKCHAIN_OBJ_Block_load(client_session->block, digest);
            BLOCKCHAIN_OBJ_Block_load(client_session->events.block, digest);
            // Send the session block to the client
            mg_send(c, client_session->block, client_session->block->headers.size);
            break;
        }
        // An MG_EV_READ event occurs when the client writes data to the server.
        case MG_EV_READ:
        {
            // Receive and validate a client-generated session block
            // This data of this block should be a UUID identical to the one generated in the MG_EV_ACCEPT event
            response = BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)c->recv.buf) && memcmp(SERVER_SESSION.digest, ((BLOCKCHAIN_OBJ_Block *)c->recv.buf), 64) && strcmp(uuid, (char *)c->recv.buf + sizeof(BLOCKCHAIN_OBJ_BlockHeaders)) == 0;
            // Save the block and load it as an event if it is valid
            if (response)
            {
                BLOCKCHAIN_OBJ_Block_save((BLOCKCHAIN_OBJ_Block *)c->recv.buf);
                byte digest[64] = {0};
                BLOCKCHAIN_OBJ_Block_hash((BLOCKCHAIN_OBJ_Block *)c->recv.buf, digest);
                BLOCKCHAIN_OBJ_LinkedBlock_next(&client_session->events);
                BLOCKCHAIN_OBJ_Block_load(client_session->events.block, digest);
            }
            // Free the connection buffer
            mg_iobuf_delete(&c->recv, c->recv.len);
            // Update the connection status
            status = (response ? BLOCKCHAIN_SERVER_ROUTING:BLOCKCHAIN_SERVER_CLOSING);
            // Send the server response
            mg_send(c, &response, 1);
            break;
        }
        // An MG_EV_WRITE event occurs when the server writes data to the client
        case MG_EV_WRITE:
        {
            // Update the client session's event list
            BLOCKCHAIN_OBJ_LinkedBlock_next(&client_session->events);
            // Create a buffer for log messages
            char msg[1024] = {0};
            // Prepare a log message
            switch (status) {
                // The server's session block was sent
                case BLOCKCHAIN_SERVER_INTRODUCTION:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN SERVER: Began client introduction\n");
                    break;
                // The client's session block was accepted
                case BLOCKCHAIN_SERVER_ROUTING:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN SERVER: Client introduced\n");
                    break;
                // The client's session block was rejected
                case BLOCKCHAIN_SERVER_CLOSING:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN SERVER: Closing client connection due to an error\n");
                    break;
                default:
                    break;
            }
            // Log the event
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_logger(stdout, msg);
            BLOCKCHAIN_OBJ_Block_mine(&SERVER_SESSION.user, SERVER_SESSION.events.previous, msg, strlen(msg) + 1, digest);
            BLOCKCHAIN_OBJ_Block_load(SERVER_SESSION.events.block, digest);
            break;
        }
        // An MG_EV_CLOSE event occurs when the connection to the client is closed
        case MG_EV_CLOSE:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        default:
            break;
    }
}
