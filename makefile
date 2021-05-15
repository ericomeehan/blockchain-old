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
all: cli



###############################################################################
# MARK: CLI
###############################################################################
cli: src components
	gcc -g blockchain/blockchain.c *.o -leom -lcrypto -o /usr/local/bin/blockchain
	make clean


###############################################################################
# MARK: LIB
###############################################################################



###############################################################################
# MARK: SRC
###############################################################################
src:
	gcc -c blockchain/src/mongoose/mongoose.c
	gcc -c blockchain/src/sqlite/sqlite3.c




###############################################################################
# MARK: COMPONENTS
###############################################################################
components: interfaces objects database server

interfaces:
	gcc -c blockchain/interfaces/cli/*.c
	gcc -c blockchain/interfaces/cli.c

objects:
	gcc -c blockchain/objects/Account.c -o OBJ_Account.o
	gcc -c blockchain/objects/Block.c -o OBJ_Block.o

database:
	gcc -c blockchain/database/database.c

server:
	gcc -c blockchain/server/objects/objects.c
	gcc -c blockchain/server/protocols/*.c
	gcc -c blockchain/server/routes/*.c
	gcc -c blockchain/server/server.c -o SRV_server.o



###############################################################################
# MARK: CLEAN
###############################################################################
clean:
	rm *.o
