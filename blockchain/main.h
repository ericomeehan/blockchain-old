//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// test.h
//
// Eric Meehan
// 5/3/21
//
//

#ifndef main_h
#define main_h

#include <libeom/libeom.h>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <sqlite3.h>

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

static sqlite3 *database;

#endif /* main_h */
