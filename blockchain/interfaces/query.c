//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// query.c
//
// Eric Meehan
// 5/12/21
//
//

#include "query.h"
#include "../objects/Block.h"

int query(int argc, const char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    
    int from = 0;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "from") == 0)
        {
            if (from)
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
                return EXIT_FAILURE;
            }
            from = i;
        }
    }
    
    if (strcmp(argv[2], "collect") == 0)
    {
        Block *block = NULL;
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < (from == 0 ? argc:from); i += 1)
        {
            trials += 1;
            if (!load_block(block, (byte *)argv[i]))
            {
                for (int node = (from == 0 ? 123:from + 1); node < (from == 0 ? 123:argc); node += 1)
                {
                    // MARK: CLIENT SIDE COLLECT IMPLEMENTATION
                    // Output hashes
                    successes += 1;
                }
            }
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of blocks were queried\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else if (strcmp(argv[2], "index") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < (from == 0 ? argc:from); i += 1)
        {
            trials += 1;
            for (int node = (from == 0 ? 123:from + 1); node < (from == 0 ? 123:argc); node += 1)
            {
                // MARK: CLIENT SIDE INDEX IMPLEMENTATION
                // Output SQL queries
                successes += 1;
            }
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of blocks were queried\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
}
