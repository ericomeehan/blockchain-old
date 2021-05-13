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
all: cli lib

###############################################################################
# MARK: COMMAND LINE APPLICATION
###############################################################################
cli: interfaces objects
	gcc -g blockchain/blockchain.c resources/sqlite3.c Account.o Block.o Client.o Server.o account.o create.o query.o read.o server.o share.o -leom -lcrypto -lpthread -ldl -o /usr/local/bin/blockchain

###############################################################################
# MARK: LIBRARY
###############################################################################
lib: objects
	ar rcs libblockchain.a Account.o Block.o Client.o Server.o

###############################################################################
# MARK: INTERFACES
###############################################################################
interfaces: account create query read server share

account:
	gcc -c blockchain/interfaces/account.c

create:
	gcc -c blockchain/interfaces/create.c

query:
	gcc -c blockchain/interfaces/query.c

read:
	gcc -c blockchain/interfaces/read.c

server:
	gcc -c blockchain/interfaces/server.c

share:
	gcc -c blockchain/interfaces/share.c

###############################################################################
# MARK: OBJECTS
###############################################################################
objects: account block client server

account:
	gcc -c blockchain/objects/Account.c

block:
	gcc -c blockchain/objects/Block.c

client:
	gcc -c blockchain/objects/Client.c

server:
	gcc -c blockchain/objects/Server.c

###############################################################################
# MARK: UTILITIES
###############################################################################
utilities:
	gcc -c blockchain/utilities/Utilities.c

###############################################################################
# MARK: CLEAN
###############################################################################

# Remove all .o files
clean:
	rm *.o
