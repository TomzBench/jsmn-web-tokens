#include "crypto_mbedtls.h"

int
crypto_base64_encode(
    char* dst,
    uint32_t dst_len,
    uint32_t* out_len,
    const char* src,
    uint32_t src_len)
{
    return mbedtls_base64_encode(
        (byte*)dst, dst_len, (size_t*)out_len, (byte*)src, src_len);
}

int
crypto_base64_dencode(
    char* dst,
    uint32_t dst_len,
    uint32_t* out_len,
    const char* src,
    uint32_t src_len)
{
    return mbedtls_base64_decode(
        (byte*)dst, dst_len, (size_t*)out_len, (byte*)src, src_len);
}
