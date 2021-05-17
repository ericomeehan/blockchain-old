# blockchain
#
# Makefile for blockchain.
# Provides options for making the command line executable, or the static library.
#
# 2021-05-10
# Eric Meehan

###############################################################################
# MARK: ALL
###############################################################################
all: cli lib clean



###############################################################################
# MARK: CLI
###############################################################################
cli: src components
	gcc -g blockchain/blockchain.c *.o -leom -lcrypto -o /usr/local/bin/blockchain



###############################################################################
# MARK: LIB
###############################################################################
lib: components
	ar rcs /usr/local/lib/libchain.a *.o



###############################################################################
# MARK: SRC
###############################################################################
src:
	gcc -c blockchain/src/mongoose/mongoose.c
	gcc -c blockchain/src/sqlite/sqlite3.c



###############################################################################
# MARK: COMPONENTS
###############################################################################
components: client interfaces objects database server utilities

client: client_protocols client_routes
	gcc -c blockchain/client/client.c

client_protocols:
	gcc -c blockchain/client/protocols/exchange.c -o CLNT_exchange.o
	gcc -c blockchain/client/protocols/introduction.c -o CLNT_introduction.o
	gcc -c blockchain/client/protocols/routing.c -o CLNT_routing.o
	gcc -c blockchain/client/protocols/close.c -o CLNT_close.o

client_routes:
	gcc -c blockchain/client/routes/get.c -o CLNT_get.o
	gcc -c blockchain/client/routes/put.c -o CLNT_put.o
	gcc -c blockchain/client/routes/sql.c -o CLNT_sql.o

interfaces:
	gcc -c blockchain/interfaces/cli/*.c
	gcc -c blockchain/interfaces/cli.c

objects:
	gcc -c blockchain/objects/account.c -o OBJ_Account.o
	gcc -c blockchain/objects/block.c -o OBJ_Block.o
	gcc -c blockchain/objects/session.c -o OBJ_Session.o

database:
	gcc -c blockchain/database/database.c

server:
	gcc -c blockchain/server/protocols/*.c
	gcc -c blockchain/server/routes/*.c
	gcc -c blockchain/server/server.c -o SRV_server.o

utilities:
	gcc -c blockchain/utilities/logger.c


###############################################################################
# MARK: CLEAN
###############################################################################
clean:
	rm *.o
