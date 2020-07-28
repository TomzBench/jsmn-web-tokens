#ifndef CRYPTO_OPENSSL_H
#define CRYPTO_OPENSSL_H

#include "jsmn_tokens.h"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int crypto_sign(
        char* dst,
        const char* p,
        uint32_t plen,
        const char* key,
        uint32_t keylen,
        JSMN_ALG alg);

#ifdef __cplusplus
}
#endif
#endif
