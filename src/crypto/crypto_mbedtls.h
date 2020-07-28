#ifndef CRYPTO_MBEDTLS_H
#define CRYPTO_MBEDTLS_H

#include "jsmn_tokens.h"

#include "mbedtls/md.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int crypto_sign(
        char* dst,
        const char* p,
        uint32_t plen,
        const byte* key,
        uint32_t keylen,
        JSMN_ALG alg);

#ifdef __cplusplus
}
#endif
#endif
