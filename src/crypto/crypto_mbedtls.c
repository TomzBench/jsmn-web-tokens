#include "crypto_mbedtls.h"
#include "base64.h"

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

static int
sha256(const char* input, uint32_t ilen, char* output)
{
    return mbedtls_sha256_ret((byte*)input, ilen, (byte*)output, 0);
}

static int
hmac_sha(
    char* dst,
    const char* p,
    uint32_t plen,
    const byte* key,
    uint32_t keylen,
    JSMN_ALG alg)
{
    int err = -1;
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t type;
    switch (alg) {
        case JSMN_ALG_HS256: type = MBEDTLS_MD_SHA256; break;
        case JSMN_ALG_HS384: type = MBEDTLS_MD_SHA384; break;
        case JSMN_ALG_HS512: type = MBEDTLS_MD_SHA512; break;
        default: return err;
    };

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

static int
b64uri_to_b64(char* dst, uint32_t* dlen, const char* src, uint32_t len)
{
    int err = -1;
    uint32_t i, t, z;
    if (*dlen > len + 4) {
        for (i = 0; i < len; i++) {
            switch (src[i]) {
                case '-': dst[i] = '+'; break;
                case '_': dst[i] = '/'; break;
                default: dst[i] = src[i];
            }
        }
        z = 4 - (i % 4);
        if (z < 4) {
            while (z--) dst[i++] = '=';
        }
        *dlen = i;
        err = 0;
    }
    return err;
}

int
crypto_base64uri_encode(
    char* dst,
    uint32_t dst_len,
    uint32_t* out_len,
    const char* src,
    uint32_t src_len)
{
    // TODO UNSAFE we need to check potential length before encoding
    uint32_t l;
    l = crypto_b64encode(dst, src, src_len);
    *out_len = strlen(dst);           // TODO investigate return length
    __jsmn_assert(*out_len == l - 1); // (testing if we can use return)
    uri_encode(dst, out_len);
    return 0;
}

int
crypto_base64uri_decode(
    char* dst,
    uint32_t dlen,
    uint32_t* olen,
    const char* src,
    uint32_t slen)
{
    // TODO unsafe we need to check potential length before decoding
    int err;
    char b[JSMN_MAX_TOKEN_LEN];
    uint32_t l, blen = sizeof(b);
    err = b64uri_to_b64(b, &blen, src, slen);
    if (err) goto ERROR;
    b[blen] = 0;
    l = crypto_b64decode(dst, b);
    *olen = strlen(dst);       // TODO investigate b64_encode return length
    __jsmn_assert(*olen == l); // (testing if we can use return)
ERROR:
    return err;
}

int
crypto_sign(
    char* dst,
    const char* p,
    uint32_t plen,
    const byte* key,
    uint32_t klen,
    JSMN_ALG alg)
{
    int ret = -1;
    switch (alg) {
        case JSMN_ALG_HS256:
        case JSMN_ALG_HS384:
        case JSMN_ALG_HS512: ret = hmac_sha(dst, p, plen, key, klen, alg);
        default: break;
    }
    return ret;
}
