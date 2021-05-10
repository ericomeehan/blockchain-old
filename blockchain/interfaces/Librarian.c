//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Librarian.c
//
// Eric Meehan
// 5/8/21
//
//

#include "Librarian.h"

// MARK: DATA TYPES

typedef struct Server Server;
typedef struct ThreadPool ThreadPool;
typedef struct ServerRoute ServerRoute;
typedef struct ThreadJob ThreadJob;
typedef struct ClientServer
{
    int client;
    Server *server;
} ClientServer;

const char *librarian_help = "\nblockchain librarian\n\nusage:\n\tstart\tLaunches the Librarian server (process runs in the background).\n";

volatile sig_atomic_t active = false;


// MARK: PROTOTYPES

char * get(void *arg);
char * put(void *arg);

bool db_insert(Block *block);

// MARK: FUNCTION DEFINITIONS

// The get function is a route function that allows a client to read a file from the Librarian.
char * get(void *arg)
{
    // Cast the argument.
    ClientServer *cs = arg;
    
    // Write an affirmitive message back to the client.
    byte response = 0;
    write(cs->client, &response, 1);
    
    // Read the requested digest from the client.
    byte digest[64] = {0};
    read(cs->client, digest, 64);
    
    // Construct the block's path.
    char path[256] = {0};
    strcat(path, BLOCK_PATH);
    strcat(path, (const char *)digest);
    strcat(path, ".block");
    
    // Check that the block exists.
    FILE *block_file = fopen(path, "r");
    if (!block_file)
    {
        // Tell the client the block was not found.
        write(cs->client, &response, 1);
        close(cs->client);
        return NULL;
    }
    
    // Write the block headers to the client.
    BlockHeaders headers;
    fread(&headers, sizeof(BlockHeaders), 1, block_file);
    write(cs->client, &headers, sizeof(BlockHeaders));
    
    // Check if the client wishes to continue.
    read(cs->client, &response, 1);
    if (!response)
    {
        // Write 0 back to the client (goodbye).
        write(cs->client, &response, 1);
        close(cs->client);
        return NULL;
    }
    
    // Read the entire block into memory.
    Block *block = malloc(headers.incidentals.size);
    fread(block, headers.incidentals.size, 1, block_file);
    
    // Write the block to the client.
    write(cs->client, block, block->headers.incidentals.size);
    close(cs->client);
    
    // Remove residuals.
    free(block);
    fclose(block_file);
    
    return NULL;
}

// The put function allows others to share blocks with the Librarian for storage.
char * put(void *arg)
{
    // Cast the argument.
    ClientServer *cs = arg;
    
    // Write an affirmitive message back to the client.
    byte response = 1;
    write(cs->client, &response, 1);
    
    // Read the headers from the client.
    BlockHeaders headers;
    read(cs->client, &headers, sizeof(BlockHeaders));
    
    // Reject blocks larger than the maximum size.
    if (headers.incidentals.size > MAX_BLOCK_SIZE)
    {
        response = 0;
        write(cs->client, &response, 1);
        close(cs->client);
        return NULL;
    }

    response = 1;
    Block *block = malloc(headers.incidentals.size);
    write(cs->client, &response, 1);
    read(cs->client, block, headers.incidentals.size);
    
    // Validate the block and let the client know its status.
    bool valid = validate(block);
    write(cs->client, &valid, 1);
    close(cs->client);
    
    // Save valid blocks
    if (!valid)
    {
        return NULL;
    }
    
    // Create the path to the block file.
    char path[256] = {0};
    strcat(path, BLOCK_PATH);
    byte digest[64] = {0};
    hash_block(block, digest);
    strcat(path, (const char *)digest);
    strcat(path, ".block");
    
    // Check if the file exists - don't overwrite data.
    FILE *block_file = fopen(path, "r");
    if (block_file)
    {
        fclose(block_file);
        return NULL;
    }
    
    // Write the block to a file.
    fclose(block_file);
    block_file = fopen(path, "w");
    fwrite(block, block->headers.incidentals.size, 1, block_file);
    fclose(block_file);
    
    // Add the block headers to the database.
    if (!db_insert(block))
    {
        fprintf(stderr, "Failed to add block to database: \n%s\n", digest);
        return NULL;
    }
    
    return NULL;
}

bool db_insert(Block *block)
{
    char *sql_error = {0};
    byte digest[64];
    hash_block(block, digest);

    sqlite3_stmt *statement;
    char *sql = "INSERT INTO blocks (hash, previous, timestamp, size, key, nonce) VALUES(?, ?, ?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(database, sql, -1, &statement, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sql_error);
        return false;
    }
    sqlite3_bind_text(statement, 1, (const char *)digest, 64, NULL);
    sqlite3_bind_text(statement, 2, (const char *)block->headers.incidentals.previous_hash, 64, NULL);
    sqlite3_bind_text(statement, 3, (const char *)block->headers.incidentals.timestamp, 20, NULL);
    sqlite3_bind_blob(statement, 4, &block->headers.incidentals.size, 8, NULL);
    sqlite3_bind_blob(statement, 5, block->headers.credentials.key, 550, SQLITE_STATIC);
    sqlite3_bind_blob(statement, 6, &block->headers.credentials.nonce, 8, NULL);
    
    rc = sqlite3_step(statement);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sql_error);
        return false;
    }
    
    return true;
}

// The handler is used in a multithreadded environment to handle incoming connections.
void * handler(void *arg)
{
    // Cast the argument back to a ClientServer struct.
    ClientServer *client_server = arg;
    // Read the client's request.
    byte request;
    read(client_server->client, &request, 1);
    // Chose between a get and put request.
    ServerRoute *route = client_server->server->routes.search(&client_server->server->routes, &request, 1);
    // Execute the route function.
    route->route_function(client_server);
    // Free the ClientServer object.
    free(client_server);
    // Thread functions must return a pointer, so this function returns NULL.
    return NULL;
}

// Terminate stops the server loop and destroys all threads for a clean exit.
void terminate(int signum)
{
    active = false;
}

// The launch sequence contains an infinite loop that accepts client connections and passes them off to a ThreadPool.
bool launch(void *arg)
{
    Server *server = arg;
    // Set up the ability to signal stop.
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGTERM, &action, NULL);
    active = true;
    // Initialize a thread pool to handle clients.
    ThreadPool thread_pool = thread_pool_constructor(NUM_THREADS);
    // Cast some of the member variables from the server.
    struct sockaddr *sock_addr = (struct sockaddr *)&server->address;
    socklen_t address_length = (socklen_t)sizeof(server->address);
    // An infinite loop allows the server to continuously accept new clients.
    while (active)
    {
        // Create an instance of the ClientServer struct.
        ClientServer *client_server = malloc(sizeof(ClientServer));
        // Accept an incoming connection.
        client_server->client = accept(server->socket, sock_addr, &address_length);
        client_server->server = server;
        // Pass the client off to the thread pool.
        ThreadJob job = thread_job_constructor(handler, client_server);
        thread_pool.add_work(&thread_pool, job);
    }
    // Destroy the thread pool.
    thread_pool_destructor(&thread_pool);
    return true;
}

// MARK: MAIN

// The Librarian handles the storage and retrieval of blocks.
int librarian(int argc, const char **argv)
{
    Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, 256);
    byte routes[] = {0, 1};
    server.register_routes(&server, get, (char *)routes);
    server.register_routes(&server, put, (char *)routes+1);
    
    if (strcmp(argv[0], "start") == 0)
    {
        if (launch(&server))
        {
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stdout, "%s", librarian_help);
        return EXIT_FAILURE;
    }
}
