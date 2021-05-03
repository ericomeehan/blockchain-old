//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Account.c
//
// Eric Meehan
// 5/2/21
//
//

#include "Account.h"

#include <string.h>
#include <unistd.h>

#include <openssl/pem.h>


int pem_password_callback(char *buf, int max_len, int flag, void *ctx)
{
    char *password = getpass("Password: ");
    int len = (int)strlen(password);
    printf("%s\n", password);
    if(len > max_len)
        return 0;

    memcpy(buf, password, len);
    return len;
}

struct Account activate(char *public_key_file, char *private_key_file)
{
    struct Account account;
    
    FILE *key_file = fopen(public_key_file, "r");
    account.public_key = NULL;
    PEM_read_PUBKEY_ex(key_file, &account.public_key, NULL, NULL, NULL, NULL);
    fclose(key_file);
    
    key_file = fopen(private_key_file, "r");
    account.private_key = NULL;
    PEM_read_PrivateKey(key_file, &account.private_key, NULL, NULL);
    fclose(key_file);
    
    return account;
}
