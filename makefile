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
cli: interfaces objects utilities
	gcc -g blockchain/main.c sqlite3.c Utilities.o Account.o Block.o Author.o Librarian.o -leom -lcrypto -lpthread -ldl -o /usr/local/bin/blockchain

###############################################################################
# MARK: LIBRARY
###############################################################################
lib: interfaces objects utilities
	ar rcs libchain.a Author.o Librarian.o Block.o Utilities.o

###############################################################################
# MARK: INTERFACES
###############################################################################
interfaces: author librarian

author:
	gcc -c blockchain/interfaces/Author.c

librarian:
	gcc -c blockchain/interfaces/Librarian.c

###############################################################################
# MARK: OBJECTS
###############################################################################
objects: account block

account:
	gcc -c blockchain/accounts/Account.c

block:
	gcc -c blockchain/blocks/Block.c

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
