//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Account.h
//
// Eric Meehan
// 5/2/21
//
//

#ifndef Account_h
#define Account_h

#include <openssl/rsa.h>

struct Account
{
    EVP_PKEY *public_key;
    EVP_PKEY *private_key;
};

void create_account(void);
struct Account activate(char *public_key_file, char *private_key_file);
void deactivate(void);
short RSASign(EVP_PKEY* rsa, const unsigned char* Msg, size_t MsgLen, unsigned char** EncMsg, size_t* MsgLenEnc);
short RSAVerifySignature( EVP_PKEY* rsa, unsigned char* MsgHash, size_t MsgHashLen, const char* Msg, size_t MsgLen, short* Authentic);
#endif /* Account_h */
