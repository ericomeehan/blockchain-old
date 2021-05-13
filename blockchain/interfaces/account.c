//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// account.c
//
// Eric Meehan
// 5/12/21
//
//

#include "account.h"

int account(int argc, const char **argv)
{
    Account user;
    if (argc != 3)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[2], "create"))
    {
        if (!create_account(&user, (char *)argv[3]))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Failed to create account %s\n", argv[3]);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Account %s created\n", argv[3]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
}
