//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Utilities.h
//
// Eric Meehan
// 5/10/21
//
//

/*
 Some generic utilities that are useful throughout the project, but don't belong anywhere on their own.
 */

#ifndef Utilities_h
#define Utilities_h

#include "main.h"

#include "Account.h"

// MARK: FUNCTION PROTOTYPES

bool initialize(void);
bool connect_to_database(void);
bool create_database_tables(void);
bool user_is_active(Account *user);

// MARK: HELP MESSAGES

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
"For more documentation, consult the README or visit https://github.com/ericomeehan/blockchain";

#endif /* Utilities_h */
