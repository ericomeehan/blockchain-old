//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Author.c
//
// Eric Meehan
// 5/6/21
//
//

#include "Author.h"
#include "../blocks/Block.h"

int author(int argc, const char **argv, Account *user)
{
    // Data will be coming from a file.
    if (strcmp(argv[3], "-f") == 0)
    {
        Block *previous = NULL;
        if (!load_block(previous, (byte *)argv[5]))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Could not load previous block %s\n", argv[5]);
            return EXIT_FAILURE;
        }
        
        FILE *data_file = fopen(argv[4], "r");
        if (!data_file)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: File %s not found\n", argv[4]);
            return EXIT_FAILURE;
        }
        byte hash[64];
        unsigned long data_size = get_file_size((char *)argv[4]);
        byte data[data_size];
        fread(data, data_size, 1, data_file);
        fclose(data_file);
        if (!mine(user, previous, data, data_size, hash))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Mining process failed\n");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%s\n", hash);
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[3], "-b") == 0)
    {
        Block *previous = NULL;
        if (!load_block(previous, (byte *)argv[5]))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Could not load previous block %s", argv[4]);
            return EXIT_FAILURE;
        }
        byte hash[64];
        if (!mine(user, previous, (void *)argv[6], strtoul(argv[5], NULL, 10), hash))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Mining process failed\n");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%s\n", hash);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "help");
        return EXIT_FAILURE;
    }
    return 1;
}
