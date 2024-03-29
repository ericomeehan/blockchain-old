//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// CLI_READ.c
//
// Eric Meehan
// 5/14/21
//
//

#include "read.h"
#include "../../objects/block.h"

int BLOCKCHAIN_CLI_read(int argc, const char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    BLOCKCHAIN_OBJ_Block *block = NULL;
    if (strcmp(argv[2], "block") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < argc; i++)
        {
            trials += 1;
            if (!BLOCKCHAIN_OBJ_Block_load(block, (byte *)argv[i]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load block %s\n", argv[i]);
                continue;
            }
            write(STDOUT_FILENO, block, block->headers.size);
            free(block);
            successes += 1;
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of blocks were delivered\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else if (strcmp(argv[2], "headers") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < argc; i++)
        {
            trials += 1;
            if (!BLOCKCHAIN_OBJ_Block_load(block, (byte *)argv[i]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load block %s\n", argv[i]);
                continue;
            }
            write(STDOUT_FILENO, &block->headers, sizeof(BLOCKCHAIN_OBJ_BlockHeaders));
            free(block);
            successes += 1;
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of block headers were delivered\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else if (strcmp(argv[2], "data") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < argc; i++)
        {
            trials += 1;
            if (!BLOCKCHAIN_OBJ_Block_load(block, (byte *)argv[i]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load block %s\n", argv[i]);
                continue;
            }
            write(STDOUT_FILENO, &block->data, block->headers.size - sizeof(BLOCKCHAIN_OBJ_BlockHeaders));
            free(block);
            successes += 1;
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of block data was delivered\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
}
