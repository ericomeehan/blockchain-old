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
// 5/14/21
//
//

#include "exchange.h"
#include "../../utilities/logger.h"

void BLOCKCHAIN_SRV_PTCL_exchange(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    // Each stage of the protocol utilizes the connection label to store a pointer to the client session, a status label, and the session uuid
    BLOCKCHAIN_OBJ_Session *client_session = (BLOCKCHAIN_OBJ_Session *)&c->label[BLOCKCHAIN_CONNECTION_LABEL_SESSION];
    void (*route)(struct mg_connection *, int, void *, void *) = (void (*)(struct mg_connection *, int, void *, void *))&c->label[BLOCKCHAIN_CONNECTION_LABEL_ROUTE];
    byte status = c->label[BLOCKCHAIN_CONNECTION_LABEL_STATUS];
    char *uuid = &c->label[BLOCKCHAIN_CONNECTION_LABEL_UUID];
    
    byte response = 0;
    
    switch (ev) {
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
        case MG_EV_ACCEPT:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        // An MG_EV_READ event occurs when the client writes data to the server.
        case MG_EV_READ:
            // Pass handling off to route
            route(c, ev, ev_data, fn_data);
            break;
        // An MG_EV_WRITE event occurs when the server writes data to the client
        case MG_EV_WRITE:
        {
            // Update the client session's event list
            BLOCKCHAIN_OBJ_LinkedBlock_next(&client_session->events);
            // Create a buffer for the message
            char msg[1024] = {0};
            // Prepare the message
            sprintf(msg, SERVER_LOGGING_FORMAT, uuid, c->peer.ip, c->peer.ip6, c->peer.is_ip6, c->peer.port, status, ev, "N/A", response, "BLOCKCHAIN SERVER: Route function executed\n");
            // Log the message
            byte digest[64] = {0};
            BLOCKCHAIN_UTIL_logger(stdout, msg);
            BLOCKCHAIN_OBJ_Block_mine(&SERVER_SESSION.user, client_session->events.previous, msg, strlen(msg) + 1, digest);
            BLOCKCHAIN_OBJ_Block_load(client_session->events.block, digest);
        }
        // An MG_EV_CLOSE event occurs when the connection to the client is closed
        case MG_EV_CLOSE:
            status = BLOCKCHAIN_SERVER_CLOSING;
            break;
        default:
            break;
    }
}
