#ifndef CRYPTO_MBEDTLS_H
#define CRYPTO_MBEDTLS_H

#include "jsmn_web_token_private.h"

#include "mbedtls/base64.h"

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

#ifdef __cplusplus
}
#endif
#endif
