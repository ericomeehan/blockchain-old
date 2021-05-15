//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// BLOCKCHAIN_DB_STUP_commands.h
//
// Eric Meehan
// 5/14/21
//
//

#ifndef BLOCKCHAIN_DB_STUP_commands_h
#define BLOCKCHAIN_DB_STUP_commands_h

const char *BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_BLOCKS =
"CREATE TABLE IF NOT EXISTS blocks("
"hash VARCHAR PRIMARY KEY NOT NULL UNIQUE, "
"previous VARCHAR NOT NULL, "
"timestamp VARCHAR NOT NULL, "
"size BIGINT NOT NULL, "
"difficulty BIGINT NOT NULL, "
"author VARCHAR NOT NULL, "
"nonce BIGINT NOT NULL, "
"FOREIGN KEY(previous) REFERENCES blocks(hash)"
");";

const char *BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_NODES =
"CREATE TABLE IF NOT EXISTS nodes("
"public_key VARCHAR PRIMARY KEY NOT NULL UNIQUE, "
"name VARCHAR DEFAULT \"anonymous\","
"last_ip VARCHAR DEFAULT \"255.255.255.255\", "
"last_connection VARCHAR DEFAULT \"20000-01-01 00:00:00\""
");";

const char *BLOCKCHAIN_DB_STUP_CMD_CREATE_TABLE_WALLETS =
"CREATE TABLE IF NOT EXISTS wallets("
"public_key VARCHAR PRIMARY KEY NOT NULL UNIQUE, "
"balance DOUBLE DEFAULT 0.0"
");";

const char *BLOCKCHAIN_DB_CMD_TEMPLATE_INSERT_INTO_BLOCKS =
"IF NOT EXISTS(SELECT hash FROM blocks WHERE hash = %s) "
"INSERT INTO blocks (hash, previous, timestamp, size, difficulty, author, nonce) VALUES (%s, %s, %s, %lu, %lu, %s, %lu);";

const char *BLOCKCHAIN_DB_CMD_TEMPLATE_INSERT_INTO_NODES =
"IF EXISTS(SELECT public_key FROM nodes WHERE public_key = %s) "
"UPDATE nodes SET name = %s, last_ip = %s, last_connection = %s WHERE public_key = %s "
"ELSE "
"INSERT INTO nodes (public_key, name, last_ip, last_connection) VALUES (%s, %s, %s, %s);";

#endif /* BLOCKCHAIN_DB_STUP_commands_h */
