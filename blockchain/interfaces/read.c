//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// read.c
//
// Eric Meehan
// 5/12/21
//
//

#include "read.h"
#include "../objects/Block.h"

int read(int argc, const char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    Block *block = NULL;
    if (strcmp(argv[2], "block") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 3; i < argc; i++)
        {
            trials += 1;
            if (!load_block(block, (byte *)argv[i]))
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
            if (!load_block(block, (byte *)argv[i]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load block %s\n", argv[i]);
                continue;
            }
            write(STDOUT_FILENO, &block->headers, sizeof(BlockHeaders));
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
            if (!load_block(block, (byte *)argv[i]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to load block %s\n", argv[i]);
                continue;
            }
            write(STDOUT_FILENO, &block->data, block->headers.size - sizeof(BlockHeaders));
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
