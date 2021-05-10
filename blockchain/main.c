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
#include "interfaces/Librarian.h"
#include "Author.h"

const char *main_help = "\nblockchain\n\nusage:\n\taccount\t\tAllows activation, deactivation, and creation of accounts.  An account must be active for other modules to function.\n\tlibrarian\t\tStarts and stops server functionality.  This must be active for other modules to function.\n\tauthor\t\tCreates blocks out of files specified by argument or data pipe.\n\tpublisher\t\tShares a block with the network.\n";

int interpreter(int argc, const char **argv);
bool initialize(void);
bool user_is_active(Account *user);
bool initialize_database(void);


int main(int argc, const char **argv)
{
    if (!initialize())
    {
        return EXIT_FAILURE;
    }
    if (isatty(fileno(stdin)))
    {
        terminal(argc, argv, interpreter, NULL);
    }
    else
    {
        char path[256] = {0};
        strcat(path, (char *)PIPE_PATH);
        strcat(path, argv[argc - 1]);
        strcat(path, ".fifo");
        terminal(argc, argv, interpreter, path);
    }
}

bool user_is_active(Account *user)
{
    return user->private_key && user->public_key;
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
        fprintf(stdout, "%s", main_help);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool initialize()
{
    // Load the configuration file.
    // Load the database.
    if (!initialize_database())
    {
        return false;
    }
    return true;
}

bool initialize_database()
{
    int rc = sqlite3_open(DATABASE_PATH, &database);
    if (rc)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return false;
    }
    
    char *sql = "CREATE TABLE IF NOT EXISTS blocks \
    (\
    hash TEXT PRIMARY KEY NOT NULL,\
    timestamp TEXT NOT NULL,\
    size INTEGER NOT NULL,\
    key BLOB NOT NULL,\
    nonce INTEGER NOT NULL,\
    FOREIGN KEY(previous) REFERENCES blocks(hash));";
    char *sql_error = {0};
    rc = sqlite3_exec(database, sql, NULL, NULL, &sql_error);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sql_error);
        return false;
    }
    
    return true;
}
