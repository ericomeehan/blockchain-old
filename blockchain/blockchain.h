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

#include "server/mongoose.h"
#include "server/database/sqlite3.h"

#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static const char *PIPE_PATH = "/Users/eric/Desktop/";
static char *BLOCK_PATH = "/Users/eric/Desktop/";
static const char *PROFILE_PATH = "/Users/eric/Desktop/";
static const char *DATABASE_PATH = "/Users/eric/Desktop/blockchain.db";

static const unsigned int PORT = 1248;
static const unsigned int NUM_THREADS = 32;

static const unsigned long MAX_BLOCK_SIZE = 1000000000;

static const char *URL = "tcp://0.0.0.0.0:%d/";
static struct mg_mgr *SERVER;
static sqlite3 *DATABASE;

#endif /* blockchain_h */
