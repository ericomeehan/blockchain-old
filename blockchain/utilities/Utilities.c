//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Utilities.c
//
// Eric Meehan
// 5/10/21
//
//

#include "Utilities.h"

// MARK: GLOBAL INITIALIZER

bool initialize(void)
{
    if (!connect_to_database())
    {
        return false;
    }
    if (!create_database_tables())
    {
        return false;
    }
    return true;
}

// MARK: DATABASE FUNCTIONS

bool connect_to_database(void)
{
    // SQLite will create a new database file if one does not exist.
    if (sqlite3_open(DATABASE_PATH, &database))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return false;
    }
    return true;
}

bool create_database_tables(void)
{
    // Creates the database tables if they do not already exist.
    char *sqlerr = {0};
    char *enable_foreign_keys = "PRAGMA foreign_keys = ON;";
    if (sqlite3_exec(database, enable_foreign_keys, NULL, NULL, &sqlerr) != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sqlerr);
        return false;
    }
    
    char *create_blocks_table = "CREATE TABLE IF NOT EXISTS blocks \
    (\
    hash TEXT PRIMARY KEY NOT NULL, \
    previous TEXT NOT NULL,\
    timestamp TEXT NOT NULL, \
    size INTEGER NOT NULL, \
    key BLOB NOT NULL,\
    nonce INTEGER NOT NULL\
    );";
    if (sqlite3_exec(database, create_blocks_table, NULL, NULL, &sqlerr) != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sqlerr);
        return false;
    }
    return true;
}

// MARK: ACCOUNT FUNCTIONS

bool user_is_active(Account *user)
{
    return user->private_key && user->public_key;
}

