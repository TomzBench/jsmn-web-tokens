#include "crypto_mbedtls.h"

static void
uri_encode(char* str, uint32_t* len)
{
    uint32_t i, t;

    for (i = t = 0; i < *len; i++) {
        switch (str[i]) {
            case '+': str[t++] = '-'; break;
            case '/': str[t++] = '_'; break;
            case '=': break;
            default: str[t++] = str[i];
        }
    }
    *len = t;
}
int
crypto_base64_encode(
    char* dst,
    uint32_t dst_len,
    uint32_t* out_len,
    const char* src,
    uint32_t src_len)
{
    int err = mbedtls_base64_encode(
        (byte*)dst, dst_len, (size_t*)out_len, (byte*)src, src_len);
    if (!err) uri_encode(dst, out_len);
    return err;
}

int
crypto_base64_dencode(
    char* dst,
    uint32_t dst_len,
    uint32_t* out_len,
    const char* src,
    uint32_t src_len)
{
    int err = mbedtls_base64_decode(
        (byte*)dst, dst_len, (size_t*)out_len, (byte*)src, src_len);
    return err;
}

int
crypto_sha256(const char* input, uint32_t ilen, char* output)
{
    return mbedtls_sha256_ret((byte*)input, ilen, (byte*)output, 0);
}

int
crypto_hmac256(
    char* dst,
    const char* p,
    uint32_t plen,
    const byte* key,
    uint32_t keylen)
{
    int err = -1;
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&ctx);
    err = mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(type), 1);
    if (err) goto ERROR;

    err = mbedtls_md_hmac_starts(&ctx, key, keylen);
    if (err) goto ERROR;

    err = mbedtls_md_hmac_update(&ctx, (const byte*)p, plen);
    if (err) goto ERROR;

    err = mbedtls_md_hmac_finish(&ctx, (byte*)dst);
    if (err) goto ERROR;

    err = 0;
ERROR:
    mbedtls_md_free(&ctx);
    return err;
}
