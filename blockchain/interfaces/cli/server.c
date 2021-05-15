//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// CLI_server.c
//
// Eric Meehan
// 5/14/21
//
//

#include "server.h"
#include "../../server/server.h"

int BLOCKCHAIN_CLI_server(int argc, const char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    
    if (strcmp(argv[2], "start") == 0)
    {
        if (!BLOCKCHAIN_SRV_initialize())
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Failed to start server\n");
            return EXIT_FAILURE;
        }
        BLOCKCHAIN_SRV_launch();
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
}
