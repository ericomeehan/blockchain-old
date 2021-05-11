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
            fprintf(stderr, "No arguments given...  How did you do that?\n");
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
    
    if (strcmp(argv[0], "account"))
    {
        if (strcmp(argv[1], "activate") == 0)
        {
            if (!activate(&user, (char *)argv[2]))
            {
                fprintf(stderr, "Failed to activate account %s.\n", argv[2]);
                return EXIT_FAILURE;
            }
            else
            {
                fprintf(stdout, "Account %s activated.\n", argv[2]);
                return EXIT_SUCCESS;
            }
        }
        else if (strcmp(argv[1], "deactivate") == 0)
        {
            if (!deactivate(&user))
            {
                fprintf(stderr, "Failed to deactivate account %s.\n", argv[2]);
                return EXIT_FAILURE;
            }
            else
            {
                fprintf(stdout, "Account %s deactivated.", argv[2]);
                return EXIT_SUCCESS;
            }
        }
        
    }
    else if (strcmp(argv[0], "librarian") == 0)
    {
        if (!user_is_active(&user))
        {
            fprintf(stderr, "Login required.  Use: 'blockchain account activate [username]'\n");
            return EXIT_FAILURE;
        }
        return librarian(argc - 1, argv + 1);
    }
    else if (strcmp(argv[0], "author"))
    {
        if (!user_is_active(&user))
        {
            fprintf(stderr, "Login required.  User 'blockchain account activate [username]'\n");
            return EXIT_FAILURE;
        }
        else
        {
            return author(argc - 1, argv + 1);
        }
    }
    else
    {
        fprintf(stdout, "%s", help);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
