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

// Thanks to:
/*
 https://gist.github.com/irbull/08339ddcd5686f509e9826964b17bb59
 
 */

#include "Account.h"

#include <string.h>
#include <unistd.h>

#include <openssl/pem.h>
#include <openssl/evp.h>


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

short RSASign( EVP_PKEY *rsa, const unsigned char* Msg, size_t MsgLen, unsigned char** EncMsg, size_t* MsgLenEnc)
{
      EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
      EVP_PKEY* priKey  = rsa;
      //EVP_PKEY_fromdata(priKey, rsa);
      if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_sha512(), NULL,priKey)<=0) {
          return 0;
      }
      if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
          return 0;
      }
      if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
          return 0;
      }
      *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
      if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
          return 0;
      }
      //EVP_MD_CTX_cleanup(m_RSASignCtx);
      return 1;
}

short RSAVerifySignature( EVP_PKEY* rsa, unsigned char* MsgHash, size_t MsgHashLen, const char* Msg, size_t MsgLen, short* Authentic) {
  *Authentic = 0;
  EVP_PKEY* pubKey  = rsa;
  EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

  if (EVP_DigestVerifyInit(m_RSAVerifyCtx,NULL, EVP_sha512(),NULL,pubKey)<=0) {
    return 0;
  }
  if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
    return 0;
  }
  int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
  if (AuthStatus==1) {
    *Authentic = 0;
    return 1;
  } else if(AuthStatus==0){
    *Authentic = 0;
    return 1;
  } else{
    *Authentic = 0;
    return 0;
  }
}

struct Account activate(char *public_key_file, char *private_key_file)
{
    struct Account account;
    
    FILE *key_file = fopen(public_key_file, "r");
    account.public_key = NULL;
    d2i_PUBKEY_fp(key_file, &account.public_key);
    fclose(key_file);
    
    key_file = fopen(private_key_file, "r");
    account.private_key = NULL;
    PEM_read_PrivateKey(key_file, &account.private_key, NULL, NULL);
    fclose(key_file);
    
    return account;
}
