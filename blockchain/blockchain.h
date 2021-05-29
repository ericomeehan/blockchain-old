//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// blockchain.h
//
// Eric Meehan
// 5/10/21
//
//

/*
 A top level header file for the blockchain library.
 Include this file when building third party applications in C.
 */

#ifndef blockchain_h
#define blockchain_h

#include <libeom/libeom.h>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "src/mongoose/mongoose.h"
#include "src/sqlite/sqlite3.h"

#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>

static const char *PIPE_PATH = "/Users/eric/Desktop/";
static const char *PROFILE_PATH = "/Users/eric/Desktop/";

static const char *LOG_PATH = "/Users/eric/Desktop/";

static const unsigned int NUM_THREADS = 32;

static char *BLOCK_PATH = "/Users/eric/Desktop/";
static const unsigned long MAX_BLOCK_SIZE = 1000000000;

struct mg_mgr SERVER;
static const char *URL = "tcp://0.0.0.0.0:%d/";
static const unsigned int PORT = 1248;
static const int SERVER_PULSE = 100;;
static const char *SERVER_LOGGING_FORMAT = "{"
"\"session\": \"%s\", "
"\"client\": { "
"\t\"ip\": %lu, "
"\t\"ip6\": %lu, "
"\t\"is_ip6\": %d, "
"\t\"port\": %lu "
"\t} "
"\"status\": %d, "
"\"event\": %d, "
"\"route\": \"%s\", "
"\"response\", %d, "
"\"notes\": \"%s\""
"}\n";

struct mg_mgr *CLIENT;
static const int CLIENT_PULSE = 1000;
static const char *CLIENT_LOGGING_FORMAT = "{"
"\"session\": \"%s\", "
"\"server\": { "
"\t\"ip\": %lu, "
"\t\"ip6\": %lu, "
"\t\"is_ip6\": %d, "
"\t\"port\": %lu "
"\t} "
"\"status\": %d, "
"\"event\": %d, "
"\"notes\": \"%s\""
"}\n";

static const char *DATABASE_PATH = "/Users/eric/Desktop/blockchain.db";
static sqlite3 *DATABASE;
static const unsigned int MAX_SQL = 4096;




static const char *HELP = "\n"
"\n"
"\n"
"\e[36m ________  ___       ________  ________  ___  __    ________  ___  ___  ________  ___  ________      \n"
"|\\   __  \\|\\  \\     |\\   __  \\|\\   ____\\|\\  \\|\\  \\ |\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\  \\|\\   ___  \\    \n"
"\\ \\  \\|\\ /\\ \\  \\    \\ \\  \\|\\  \\ \\  \\___|\\ \\  \\/  /|\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\\\ \\  \\   \n"
" \\ \\   __  \\ \\  \\    \\ \\  \\\\\\  \\ \\  \\    \\ \\   ___  \\ \\  \\    \\ \\   __  \\ \\   __  \\ \\  \\ \\  \\\\ \\  \\  \n"
"  \\ \\  \\|\\  \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\____\\ \\  \\\\ \\  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \n"
"   \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\ \\__\\\\ \\__\\\n"
"    \\|_______|\\|_______|\\|_______|\\|_______|\\|__| \\|__|\\|_______|\\|__|\\|__|\\|__|\\|__|\\|__|\\|__| \\|__|\e[0m\n"
"\n"
"\n"
"Created by Eric O Meehan\n"
"https://www.ericomeehan.com\n"
"\n"
"\n"
"\e[1mGENERAL USAGE\e[0m\n"
"\e[32mblockchain\n"
"    \e[32maccount\e[0m                                 account management\n"
"        \e[32mcreate\e[0m                                      create a new account\n"
"            \e[2m[username]\e[0m\n"
"\n"
"    \e[32mcreate\e[0m                                  content creation\n"
"        \e[32mfrom\e[0m                                        create a block from file contents\n"
"            \e[2m[/path/to/file(s)]\e[0m                                 (can specify multiple files)\n"
"        \e[32mthis\e[0m                                        create a block from incoming data\n"
"            'content'                                                  (use quotes, can specify multiple)\n"
"            \e[2m[size of content]\e[0m \e[2m[raw data]\e[0m               (not for general use)\n"
"\n"
"    \e[32mshare\e[0m                                   share content with the network\n"
"        \e[2m[hash(es)] (\e[0mwith \e[2mip_address(es)\e[0m)            specify block(s) to share and, optionally, who to share with\n"
"\n"
"    \e[32mread\e[0m                                    read data from blocks\n"
"        \e[32mblock\e[0m                                       read entire block\n"
"            \e[2mhash(es)\e[0m\n"
"        \e[32mheaders\e[0m                                     read only headers\n"
"            \e[2mhash(es)\e[0m\n"
"        \e[32mdata\e[0m                                        read data only (general use case)\n"
"            \e[2mhash(es)\e[0m\n"
"\n"
"    \e[32mquery\e[0m                                   search the blockchain network\n"
"        \e[32mblock\e[0m                                       search for a specific block on the network by hash\n"
"            \e[2mhash(es) (\e[0mfrom \e[2mip_address(es)\e[0m)\n"
"        \e[32msql\e[0m                                         perform an sql query on the network\n"
"            \e[2m[sql] (\e[0mfrom \e[2mip_address(es)\e[0m)\n"
"\n"
"\n"
"\e[1mTYPICAL STARTUP\e[0m\n"
"In order to utilize blockchain, you will need to first create and activate an account, then launch the blockchain server\n"
"If you haven’t already, create an account using:\n"
"    \e[7mblockchain account create \e[2musername\e[0m\n"
"Now activate your account:\n"
"    \e[7mblockchain account activate \e[2musername\e[0m\n"
"And launch the server:\n"
"    \e[7mblockchain start\e[0m\n"
"\n"
"For more detailed documentation, consult the README:       \e[34mhttps://github.com/ericomeehan/blockchain\e[0m\n"
"Or visit Eric’s YouTube channel for tutorials:             \e[34mhttps://youtube.com/c/EricOMeehan\e[0m\n"
"\n";

#endif /* blockchain_h */
