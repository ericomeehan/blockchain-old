//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// exchange.c
//
// Eric Meehan
// 5/15/21
//
//

#include "exchange.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_CLNT_PTCL_exchange(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
            status = BLOCKCHAIN_CLIENT_CLOSING;
            break;
        // An MG_EV_POLL event occurs by default with each iteration of the server's infinite loop.
        case MG_EV_POLL:
            break;
        // An MG_EV_RESOLVE event occurs when the resolution of a host's domain to an ip
        case MG_EV_RESOLVE:
            status = BLOCKCHAIN_CLIENT_INTRODUCTION;
            break;
        // An MG_EV_CONNECT event occurs when a host machine has been connected to
        case MG_EV_CONNECT:
            status = BLOCKCHAIN_CLIENT_INTRODUCTION;
            break;
        // An MG_EV_ACCPET event represents a new client connection
        case MG_EV_ACCEPT:
            status = BLOCKCHAIN_CLIENT_CLOSING;
            break;
        // An MG_EV_READ event occurs when the server writes data to the client.
        case MG_EV_READ:
        {
            void (*route)(struct mg_connection *, int, void *, void *) = CLIENT_ROUTES.search(&CLIENT_ROUTES, request->route, strlen((char *)request->route) + 1);
            if (route) { route(c, ev, ev_data, fn_data); }
            else { status = BLOCKCHAIN_CLIENT_CLOSING; }
            
        }
        // An MG_EV_WRITE event occurs when the client writes data to the server
        case MG_EV_WRITE:
        {
            // Update the client session's event list
            BLOCKCHAIN_OBJ_LinkedBlock_next(&request->events);
            // Create a buffer for the message
            char msg[1024] = {0};
            char note[256] = {0};
            // Prepare the message
            sprintf(note, "BLOCKCHAIN CLIENT: Data exchanged on iteration %d", iteration);
            sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, note);
            // Log the message
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_logger(stdout, msg);
            BLOCKCHAIN_OBJ_Block_mine(request->user, request->events.previous, msg, strlen(msg) + 1, digest);
            BLOCKCHAIN_OBJ_Block_load(request->events.block, digest);
            iteration += 1;
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
