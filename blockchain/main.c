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

#include "Account.h"
#include "Block.h"
#include "Librarian.h"
#include "Author.h"

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
