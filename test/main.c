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

#include "../test/unit/cryptography/account.c"

#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

int crypto_init(void);
int crypto_init()
{
  /* Load the human readable error strings for libcrypto */
  ERR_load_crypto_strings();

  /* Load all digest and cipher algorithms */
  OpenSSL_add_all_algorithms();

  /* ... Do some crypto stuff here ... */

  /* Clean up */

  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  ERR_free_strings();

  return 0;
}

int main(int argc, const char * argv[]) {
    crypto_init();
    float rate = test();
    printf("Tests completed with a %f%% success rate.\n", rate * 100);
    return 0;
}
