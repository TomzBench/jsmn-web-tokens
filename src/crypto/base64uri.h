#ifndef CRYPTO_BASE64_URI_H
#define CRYPTO_BASE64_URI_H

#include "jsmn_tokens.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int crypto_base64uri_encode(
        char* dst,
        uint32_t dst_len,
        uint32_t* out_len,
        const char* src,
        uint32_t src_len);

    int crypto_base64uri_decode(
        char* dst,
        uint32_t dst_len,
        uint32_t* out_len,
        const char* src,
        uint32_t src_len);

#ifdef __cplusplus
}
#endif
#endif
