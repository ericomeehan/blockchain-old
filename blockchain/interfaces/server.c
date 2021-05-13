//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// server.c
//
// Eric Meehan
// 5/12/21
//
//

#include "server.h"

int server(int argc, const char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    
    if (strcmp(argv[2], "start") == 0)
    {
        // MARK: SERVER LAUNCH SEQUENCE
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[2], "stop") == 0)
    {
        // MARK: SERVER SHUTDOWN SEQUENCE
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
}
