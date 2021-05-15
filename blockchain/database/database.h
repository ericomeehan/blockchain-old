//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// database.h
//
// Eric Meehan
// 5/12/21
//
//

#ifndef BLOCKCHAIN_DB_database_h
#define BLOCKCHAIN_DB_database_h

#include "../blockchain.h"

bool BLOCKCHAIN_DB_init(void);
bool BLOCKCHAIN_DB_query(char *query, int (*cb)(void *callback_argument, int num_columns, char **column_text, char **column_names), void *dt);
int BLOCKCHAIN_DB_generic_callback(void *callback_argument, int num_columns, char **column_text, char **column_names);
#endif /* BLOCKCHAIN_DB_database_h */
