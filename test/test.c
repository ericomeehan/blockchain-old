//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// main.c
//
// Eric Meehan
// 5/2/21
//
//

// This file is used for testing purposes.
// Include the test file here and call its main function.

#include "test.h"

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

int main(int argc, const char * argv[]) {
    crypto_init();
    float rate = 0;
    int tests = 1;
    
    //rate += account_unit_test();
    
    Account user;
    activate(&user, "test");
    
    //print_account(&user);
    
    rate += block_unit_test(&user);
    
    printf("Tests completed with a %f%% success rate.\n", (rate/tests) * 100);
    return 0;
}

int crypto_init()
{
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  EVP_cleanup();
  CRYPTO_cleanup_all_ex_data();
  ERR_free_strings();
  return 0;
}
