//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// CLI_account.c
//
// Eric Meehan
// 5/14/21
//
//

#include "account.h"

int BLOCKCHAIN_CLI_account(int argc, const char **argv)
{
    BLOCKCHAIN_OBJ_Account user;
    if (argc != 4)
    {
        fprintf(stderr, "BLOCKCHAIN ERROR: Command invalid, try 'blockchain help'\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[2], "create") == 0)
    {
        if (!BLOCKCHAIN_OBJ_Account_create(&user, (char *)argv[3]))
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
