//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// share.c
//
// Eric Meehan
// 5/12/21
//
//

#include "share.h"
#include "../objects/Block.h"

int share(int argc, const char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    
    int with = 0;
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "with"))
        {
            if (with)
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
                return EXIT_FAILURE;
            }
            with = i;
        }
    }
    
    // MARK: CLIENT SIDE OF PUT
}

