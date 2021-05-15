//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// database.c
//
// Eric Meehan
// 5/12/21
//
//

#include "database.h"
#include "setup/commands.h"

static bool BLOCKCHAIN_DB_connect(void);
static bool BLOCKCHAIN_DB_create_default_tables(void);
static int BLOCKCHAIN_DB_query_callback(void *unused, int count, char **data, char **columns)
{
    
    return 0;
}

bool BLOCKCHAIN_DB_init()
{
    return BLOCKCHAIN_DB_connect() && BLOCKCHAIN_DB_create_default_tables();
}

bool BLOCKCHAIN_DB_query(char *sql, int (*callback)(void *callback_argument, int num_columns, char **column_text, char **column_names), void *callback_argument)
{
    char *sqlerr = {0};
    if (!sqlite3_exec(DATABASE, sql, callback, callback_argument, &sqlerr))
    {
        fprintf(stderr, "SQL ERROR: %s\n", sqlerr);
        return false;
    }
    return true;
}

int BLOCKCHAIN_DB_generic_callback(void *callback_argument, int num_columns, char **column_text, char **column_names)
{
    for (int i = 0; i < num_columns; i++)
    {
        fprintf(stdout, "%s", column_text[i]);
        if (i != num_columns - 1)
        {
            fprintf(stdout, ", ");
        }
        else
        {
            fprintf(stdout, "\n");
        }
    }
    return EXIT_SUCCESS;
}

bool BLOCKCHAIN_DB_connect()
{
    // SQLite will create a new database file if one does not exist.
    if (sqlite3_open(DATABASE_PATH, &DATABASE))
    {
        fprintf(stderr, "SQL ERROR: Cannot open database: %s\n", sqlite3_errmsg(DATABASE));
        sqlite3_close(DATABASE);
        return false;
    }
    return true;
}


bool BLOCKCHAIN_DB_create_default_tables()
{
    char *sqlerr = {0};
    if (!sqlite3_exec(DATABASE, BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_BLOCKS, NULL, NULL, &sqlerr))
    {
        fprintf(stderr, "SQL ERROR: %s\n", sqlerr);
        return false;
    }
    if (!sqlite3_exec(DATABASE, BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_NODES, NULL, NULL, &sqlerr))
    {
        fprintf(stderr, "SQL ERROR: %s\n", sqlerr);
        return false;
    }
    if (!sqlite3_exec(DATABASE, BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_WALLETS, NULL, NULL, &sqlerr))
    {
        fprintf(stderr, "SQL ERROR: %s\n", sqlerr);
        return false;
    }
    return true;
}

bool BLOCKCHAIN_DB_insert_block(BLOCKCHAIN_OBJ_Block *block)
{
    char sql[4096] = {0};
    
    byte digest[64] = {0};
    BLOCKCHAIN_OBJ_Block_hash(block, digest);
    
    BIGNUM *value = BN_new();
    
    BN_bin2bn(digest, 64, value);
    char *hash = BN_bn2hex(value);
    
    BN_bin2bn(block->headers.previous_hash, 64, value);
    char *previous_hash = BN_bn2hex(value);

    
    BN_bin2bn(block->headers.key, 550, value);
    char *public_key = BN_bn2hex(value);
    
    sprintf(sql, BLOCKCHAIN_DB_CMD_TEMPLATE_INSERT_INTO_BLOCKS, hash, previous_hash, block->headers.timestamp, block->headers.size, public_key, block->headers.nonce);
    
    BN_free(value);
    
    return BLOCKCHAIN_DB_query(sql, NULL, NULL);
}
