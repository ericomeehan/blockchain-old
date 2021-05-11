//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// main.c
//
// Eric Meehan
// 4/10/21
//
//

#include "main.h"

#include "interfaces/Librarian.h"
#include "interfaces/Author.h"
#include "utilities/Utilities.h"

const char *help = "\n"
"\n"
"\n"
" ________  ___       ________  ________  ___  __    ________  ___  ___  ________  ___  ________      \n"
"|\\   __  \\|\\  \\     |\\   __  \\|\\   ____\\|\\  \\|\\  \\ |\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\  \\|\\   ___  \\    \n"
"\\ \\  \\|\\ /\\ \\  \\    \\ \\  \\|\\  \\ \\  \\___|\\ \\  \\/  /|\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\\\ \\  \\   \n"
" \\ \\   __  \\ \\  \\    \\ \\  \\\\\\  \\ \\  \\    \\ \\   ___  \\ \\  \\    \\ \\   __  \\ \\   __  \\ \\  \\ \\  \\\\ \\  \\  \n"
"  \\ \\  \\|\\  \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\____\\ \\  \\\\ \\  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \n"
"   \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\\\ \\__\\\n"
"    \\|_______|\\|_______|\\|_______|\\|_______|\\|__| \\|__|\\|_______|\\|__|\\|__|\\|__|\\|__|\\|__|\\|__| \\|__|\n"
"\n"
"\n"
"Created by Eric O Meehan\n"
"https://www.ericomeehan.com\n"
"\n"
"\n"
"USAGE:\n"
"blockchain                                        Application Name\n"
"    account                                            Used to create, activate, and deactivate an account\n"
"        create                                             Creates a new account\n"
"           [username]\n"
"        display                                            Prints the account's name and public key\n"
"           [username]\n"
"\n"
"    [username]                                         Specifies an account to be used\n"
"\n"
"        author                                         Module for creating blocks\n"
"            -f                                             Marks that the input will be a file\n"
"                [/path/to/file]\n"
"            -b                                             Marks that the input will be raw bytes\n"
"                [bytes]\n"
"\n"
"        publisher                                      Module for sharing blocks\n"
"            [hash of block to be shared]\n"
"                [ip addresses to be shared with]           (Optional)\n"
"\n"
"        librarian                                      Module for storing and retrieving blocks\n"
"            start                                          Starts the Librarian server\n"
"            query                                          Searches the blockchain (using SQL syntax)\n"
"                \"[query in SQL syntax]\""
"\n"
"        client                                         Module for collecting the raw bytes of a block\n"
"            [hash of block to be read]\n"
"\n"
"\n"
"GENERAL START SEQUENCE:\n"
"This sequence of commands will create a new blockchain account, start the librarian server, and then\n"
"creates and shares a block that identifies you by username on the network.  While the first two commands are\n"
"required, the third is optional; however, it demonstrates how blockchain takes advantage of pipes.\n"
"```\n"
"blockchain account create [username]\n"
"blockchain [username] librarian start\n"
"blockchain [username] author \"#mynameis $(blockchain account display [username])\"\n"
"```\n"
"For more documentation, consult the README or visit https://github.com/ericomeehan/blockchain\n";

int interpreter(int argc, const char **argv);

int main(int argc, const char **argv)
{
    if (!initialize())
    {
        return EXIT_FAILURE;
    }
    switch (argc)
    {
        // There should always be at least one argument...
        case 0:
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: No arguments given...  How did you do that?\n");
            return EXIT_FAILURE;
            break;
        }
            
        // When one argument is given (the program name), print the help message.
        case 1:
        {
            fprintf(stdout, "%s", help);
            return EXIT_SUCCESS;
        }

        // When two arguments are given, the second is the UUID that will be used to create an application pipeline... Or a cry for help.
        case 2:
        {
            if (strcmp(argv[1], "help"))
            {
                fprintf(stdout, "%s", help);
                return EXIT_SUCCESS;
                
            }
            else
            {
                // Generate a path to a pipe file using the last argument.
                char path[256] = {0};
                strcat(path, (char *)PIPE_PATH);
                strcat(path, argv[argc - 1]);
                strcat(path, ".fifo");
                // Decrement argc so that the terminal recognizes this as a piped process.
                argc -= 1;
                return terminal(argc, argv, interpreter, path);
            }
            break;
        }
            
        // If more than one argument is given, execute them as a single command with no pipeline.
        default:
        {
            return terminal(argc, argv, interpreter, NULL);
            break;
        }
            
    }
}

int interpreter(int argc, const char **argv)
{
    Account user;

    if (strcmp(argv[1], "account") == 0)
    {
        if (strcmp(argv[2], "create") == 0)
        {
            if (!account_exists((char *)argv[3]))
            {
                if (!create(&user, (char *)argv[3]))
                {
                    fprintf(stderr, "BLOCKCHAIN ERROR: Failed to create account %s\n", argv[3]);
                    return EXIT_FAILURE;
                }
                return EXIT_SUCCESS;
            }
            fprintf(stderr, "BLOCKCHAIN ERROR: Account %s already exists\n", argv[3]);
            return EXIT_FAILURE;
        }
        else if (strcmp(argv[2], "activate") == 0)
        {
            if (!account_exists((char *)argv[3]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Account %s does not exist\n", argv[3]);
                return EXIT_FAILURE;
            }
            if (!activate(&user, (char *)argv[3]))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to activate account %s.\n", argv[3]);
                return EXIT_FAILURE;
            }
            else
            {
                fprintf(stdout, "Account %s activated.\n", argv[3]);
                return EXIT_SUCCESS;
            }
        }
        else if (strcmp(argv[2], "deactivate") == 0)
        {
            if (!deactivate(&user))
            {
                fprintf(stderr, "BLOCKCHAIN ERROR: Failed to deactivate account %s.\n", argv[3]);
                return EXIT_FAILURE;
            }
            else
            {
                fprintf(stdout, "Account %s deactivated.", argv[3]);
                return EXIT_SUCCESS;
            }
        }
        else
        {
            fprintf(stderr, "%s", help);
            return EXIT_FAILURE;
        }
        
    }
    // argv[1] is now assumed to be an account that has been previously created.
    else
    {
        if (!account_exists((char *)argv[1]))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Account %s does not exist\n", argv[1]);
            return EXIT_FAILURE;
        }
        if (!activate(&user, (char *)argv[1]))
        {
            fprintf(stderr, "BLOCKCHAIN ERROR: Failed to activate account %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        
        // Examine argv[2] for next layer.
        if (strcmp(argv[2], "author") == 0)
        {
            return author(argc, argv);
        }
        else if (strcmp(argv[2], "librarian") == 0)
        {
            return librarian(argc, argv);
        }
        else if (strcmp(argv[2], "publisher") == 0)
        {
            return EXIT_FAILURE;
        }
        else if (strcmp(argv[2], "client"))
        {
            return EXIT_FAILURE;
        }
        else
        {
            fprintf(stderr, "%s", help);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
