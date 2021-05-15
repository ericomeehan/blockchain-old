//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// cli.c
//
// Eric Meehan
// 5/14/21
//
//

#include "cli.h"

#include "cli/account.h"
#include "cli/create.h"
#include "cli/query.h"
#include "cli/read.h"
#include "cli/server.h"
#include "cli/share.h"

int BLOCKCHAIN_cli(int argc, const char **argv)
{
    switch (argc) {
        case 1:
        {
            fprintf(stdout, "%s", HELP);
            return EXIT_SUCCESS;
            break;;
        }
        case 2:
        {
            fprintf(stdout, "%s", HELP);
            return EXIT_SUCCESS;
            break;
        }
        default:
        {
            if (strcmp(argv[1], "account") == 0)
            {
                return BLOCKCHAIN_CLI_account(argc, argv);
            }
            else if(strcmp(argv[1], "create") == 0)
            {
                return BLOCKCHAIN_CLI_create(argc, argv);
            }
            else if (strcmp(argv[1], "query") == 0)
            {
                return BLOCKCHAIN_CLI_query(argc, argv);
            }
            else if (strcmp(argv[1], "read") == 0)
            {
                return BLOCKCHAIN_CLI_read(argc, argv);
            }
            else if (strcmp(argv[1], "server") == 0)
            {
                return BLOCKCHAIN_CLI_server(argc, argv);
            }
            else if (strcmp(argv[1], "share") == 0)
            {
                return BLOCKCHAIN_CLI_share(argc, argv);
            }
            else
            {
                fprintf(stderr, "%s", HELP);
                return EXIT_FAILURE;
            }
            break;
        }
    }
}
