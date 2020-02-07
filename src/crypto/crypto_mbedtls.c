#include "crypto_mbedtls.h"
#include "base64.h"
#include "math.h"

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
