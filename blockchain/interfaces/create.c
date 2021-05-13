//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// create.c
//
// Eric Meehan
// 5/12/21
//
//

#include "create.h"
#include "../objects/Account.h"
#include "../objects/Block.h"

int create(int argc, const char **argv)
{
    if (argc < 5)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    
    Account user;
    char name[256] = {0};
    fprintf(stdout, "Username: ");
    fgets(name, 256, stdin);
    if (!activate_account(&user, name))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Failed to log into account %s\n", name);
        return EXIT_FAILURE;
    }
    
    
    Block *previous = NULL;
    if (!load_block(previous, (byte *)argv[2]))
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Unable to load block %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    if (strcmp(argv[3], "from") == 0)
    {
        double trials = 0;
        double successes = 0;
        for (int i = 4; i < argc; i++)
        {
            FILE *data_file = fopen(argv[i], "r");
            if (!data_file)
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Unable to load file %s\n", argv[i]);
                continue;
            }
            unsigned long size = get_file_size((char *)argv[i]);
            byte data[size];
            memset(data, 0, size);
            fread(data, size, 1, data_file);
            fclose(data_file);
            
            byte hash[64] = {0};
            if (!mine(&user, previous, data, size, hash))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to mine file %s\n", argv[i]);
                continue;
            }
            fprintf(stdout, "%s", hash);
        }
        if (successes != trials)
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Only %.2f%% of files were mined successfully\n", (successes / trials) * 100);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else if (strcmp(argv[2], "this") == 0)
    {
        if (argc == 6)
        {
            unsigned long size = strtoul(argv[4], NULL, 10);
            byte data[size];
            memset(data, 0, size);
            byte hash[64] = {0};
            if (!mine(&user, previous, (void *)argv[5], size, hash))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to mine data\n");
                return EXIT_FAILURE;
            }
            fprintf(stdout, "%s\n", hash);
            return EXIT_SUCCESS;
        }
        else
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
}
