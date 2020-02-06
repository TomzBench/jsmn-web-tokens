#ifndef CRYPTO_MBEDTLS_H
#define CRYPTO_MBEDTLS_H

#include "jsmn_tokens_private.h"

#include "mbedtls/base64.h"
#include "mbedtls/md.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned char byte;

    int crypto_base64_encode(
        char* dst,
        uint32_t dst_len,
        uint32_t* out_len,
        const char* src,
        uint32_t src_len);

    int crypto_base64_dencode(
        char* dst,
        uint32_t dst_len,
        uint32_t* out_len,
        const char* src,
        uint32_t src_len);

    int crypto_sha256(const char* input, uint32_t ilen, char* output);
    int crypto_hmac256(
        char* dst,
        const char* p,
        uint32_t plen,
        const byte* s,
        uint32_t slen);

#ifdef __cplusplus
}
#endif
#endif
