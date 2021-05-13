//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// blockchain.c
//
// Eric Meehan
// 5/12/21
//
//

#include "blockchain.h"

#include "interfaces/account.h"
#include "interfaces/create.h"
#include "interfaces/query.h"
#include "interfaces/read.c"
#include "interfaces/server.h"
#include "interfaces/share.h"


char *help = "\n"
"\n"
"\n"
"\e[36m ________  ___       ________  ________  ___  __    ________  ___  ___  ________  ___  ________      \n"
"|\\   __  \\|\\  \\     |\\   __  \\|\\   ____\\|\\  \\|\\  \\ |\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\  \\|\\   ___  \\    \n"
"\\ \\  \\|\\ /\\ \\  \\    \\ \\  \\|\\  \\ \\  \\___|\\ \\  \\/  /|\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\\\ \\  \\   \n"
" \\ \\   __  \\ \\  \\    \\ \\  \\\\\\  \\ \\  \\    \\ \\   ___  \\ \\  \\    \\ \\   __  \\ \\   __  \\ \\  \\ \\  \\\\ \\  \\  \n"
"  \\ \\  \\|\\  \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\____\\ \\  \\\\ \\  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \n"
"   \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\\\ \\__\\\n"
"    \\|_______|\\|_______|\\|_______|\\|_______|\\|__| \\|__|\\|_______|\\|__|\\|__|\\|__|\\|__|\\|__|\\|__| \\|__|\e[0m\n"
"\n"
"\n"
"Created by Eric O Meehan\n"
"https://www.ericomeehan.com\n"
"\n"
"\n"
"\e[1mGENERAL USAGE\e[0m\n"
"\e[32mblockchain\n"
"    \e[32maccount\e[0m                                 account management\n"
"        \e[32mcreate\e[0m                                      create a new account\n"
"            \e[2m[username]\e[0m\n"
"\n"
"    \e[32mcreate\e[0m                                  content creation\n"
"        \e[32mfrom\e[0m                                        create a block from file contents\n"
"            \e[2m[/path/to/file(s)]\e[0m                                 (can specify multiple files)\n"
"        \e[32mthis\e[0m                                        create a block from incoming data\n"
"            'content'                                                  (use quotes, can specify multiple)\n"
"            \e[2m[size of content]\e[0m \e[2m[raw data]\e[0m               (not for general use)\n"
"\n"
"    \e[32mshare\e[0m                                   share content with the network\n"
"        \e[2m[hash(es)] (\e[0mwith \e[2mip_address(es)\e[0m)            specify block(s) to share and, optionally, who to share with\n"
"\n"
"    \e[32mread\e[0m                                    read data from blocks\n"
"        \e[32mblock\e[0m                                       read entire block\n"
"            \e[2mhash(es)\e[0m\n"
"        \e[32mheaders\e[0m                                     read only headers\n"
"            \e[2mhash(es)\e[0m\n"
"        \e[32mdata\e[0m                                        read data only (general use case)\n"
"            \e[2mhash(es)\e[0m\n"
"\n"
"    \e[32mquery\e[0m                                   search the blockchain network\n"
"        \e[32mblock\e[0m                                       search for a specific block on the network by hash\n"
"            \e[2mhash(es) (\e[0mfrom \e[2mip_address(es)\e[0m)\n"
"        \e[32msql\e[0m                                         perform an sql query on the network\n"
"            \e[2m[sql] (\e[0mfrom \e[2mip_address(es)\e[0m)\n"
"\n"
"\n"
"\e[1mTYPICAL STARTUP\e[0m\n"
"In order to utilize blockchain, you will need to first create and activate an account, then launch the blockchain server\n"
"If you haven’t already, create an account using:\n"
"    \e[7mblockchain account create \e[2musername\e[0m\n"
"Now activate your account:\n"
"    \e[7mblockchain account activate \e[2musername\e[0m\n"
"And launch the server:\n"
"    \e[7mblockchain start\e[0m\n"
"\n"
"For more detailed documentation, consult the README:       \e[34mhttps://github.com/ericomeehan/blockchain\e[0m\n"
"Or visit Eric’s YouTube channel for tutorials:             \e[34mhttps://youtube.com/c/EricOMeehan\e[0m\n"
"\n";

int main(int argc, const char **argv)
{
    switch (argc) {
        case 1:
        {
            fprintf(stdout, "%s", help);
            break;;
        }
        case 2:
        {
            fprintf(stdout, "%s", help);
            break;
        }
        default:
        {
            if (strcmp(argv[1], "account") == 0)
            {
                return account(argc, argv);
            }
            else if(strcmp(argv[1], "create") == 0)
            {
                return create(argc, argv);
            }
            else if (strcmp(argv[1], "query") == 0)
            {
                return query(argc, argv);
            }
            else if (strcmp(argv[1], "read") == 0)
            {
                return read(argc, argv);
            }
            else if (strcmp(argv[1], "server") == 0)
            {
                return server(argc, argv);
            }
            else if (strcmp(argv[1], "share") == 0)
            {
                return share(argc, argv);
            }
            else
            {
                fprintf(stderr, "%s", help);
                return EXIT_FAILURE;
            }
            break;
        }
    }
        
}
