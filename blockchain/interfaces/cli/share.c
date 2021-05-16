//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// CLI_share.c
//
// Eric Meehan
// 5/14/21
//
//

#include "share.h"
#include "../../objects/block.h"

int BLOCKCHAIN_CLI_share(int argc, const char **argv)
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
    return 1;
    // MARK: CLIENT SIDE OF PUT
}

