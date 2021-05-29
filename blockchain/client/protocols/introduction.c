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
    BLOCKCHAIN_CLNT_OBJ_request *request = fn_data;
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    byte iteration = c->label[BLOCKCHAIN_CONNECTION_LABEL_ITERATION];
    char *uuid = &c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    
    byte response = 0;
    
    switch (ev)
    {
        // An MG_EV_ERROR event occurs when there is a problem and the client should be closed
        case MG_EV_ERROR:
            break;
        // An MG_EV_POLL event occurs by default with each iteration of the server's infinite loop.
        case MG_EV_POLL:
            break;
        // An MG_EV_RESOLVE event occurs when the resolution of a host's domain to an ip
        case MG_EV_RESOLVE:
            break;
        // An MG_EV_CONNECT event occurs when a host machine has been connected to
        case MG_EV_CONNECT:
            break;
        // An MG_EV_ACCPET event represents a new client connection
        case MG_EV_ACCEPT:
            break;
        // An MG_EV_READ event occurs when the server writes data to the client.
        // After being accepted, the server will immediately send a session block, initiating a read event
        case MG_EV_READ:
        {
            status = ((response = BLOCKCHAIN_OBJ_Block_validate((BLOCKCHAIN_OBJ_Block *)c->recv.buf)) ? BLOCKCHAIN_CLIENT_ROUTING:BLOCKCHAIN_CLIENT_CLOSING);
            iteration = 0;
            if (response)
            {
                // Save the new session block
                BLOCKCHAIN_OBJ_Block_save((BLOCKCHAIN_OBJ_Block *)c->recv.buf);
                // Load the session block into the request events
                byte digest[64] = {0};
                BLOCKCHAIN_OBJ_Block_hash((BLOCKCHAIN_OBJ_Block *)c->recv.buf, digest);
                BLOCKCHAIN_OBJ_Block_load(request->events.block, digest);
                // Create a second session block with the same UUID
                // Update the events structure
                BLOCKCHAIN_OBJ_LinkedBlock_next(&request->events);
                // Reset digest
                memset(digest, 0, 64);
                // Copy the UUID
                strcpy(uuid, (char *)&((BLOCKCHAIN_OBJ_Block *)c->recv.buf)->data);
                // Mine the new session block
                BLOCKCHAIN_OBJ_Block_mine(request->user, request->events.previous, uuid, strlen(uuid) + 1, digest);
                BLOCKCHAIN_OBJ_Block_load(request->events.block, digest);
            }
            mg_send(c, (response ? (void *)request->events.block:&response), (response ? request->events.block->headers.size:1));
        }
        // An MG_EV_WRITE event occurs when the client writes data to the server
        case MG_EV_WRITE:
        {
            // Update the client session's event list
            BLOCKCHAIN_OBJ_LinkedBlock_next(&request->events);
            // Create a buffer for log messages
            char msg[1024] = {0};
            // Prepare a log message
            switch (status) {
                // The server's session block was sent
                case BLOCKCHAIN_CLIENT_INTRODUCTION:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN CLIENT: Began server introduction");
                    break;
                // The client's session block was accepted
                case BLOCKCHAIN_CLIENT_ROUTING:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN CLIENT: Server introduced");
                    break;
                // The client's session block was rejected
                case BLOCKCHAIN_CLIENT_CLOSING:
                    sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN CLIENT: Closing server connection due to an error");
                    break;
                default:
                    break;
            }
            // Log the event
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_logger(stdout, msg);
            BLOCKCHAIN_OBJ_Block_mine(request->user, request->events.previous, msg, strlen(msg) + 1, digest);
            BLOCKCHAIN_OBJ_Block_load(request->events.block, digest);
            break;
        }
        // An MG_EV_CLOSE event occurs when the connection to the server is closed
        case MG_EV_CLOSE:
        {
            // Update the client session's event list
            BLOCKCHAIN_OBJ_LinkedBlock_next(&request->events);
            // Create a buffer for the message
            char msg[1024] = {0};
            // Prepare the message
            sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN CLIENT: Connection closed");
            // Log the message
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_logger(stdout, msg);
            BLOCKCHAIN_OBJ_Block_mine(request->user, request->events.previous, msg, strlen(msg) + 1, digest);
            BLOCKCHAIN_OBJ_Block_load(request->events.block, digest);
            break;
        }
        default:
            break;
    }
}
