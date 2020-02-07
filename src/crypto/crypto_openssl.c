#include "crypto_openssl.h"

static int
hmac_sha(
    char* dst,
    const char* p,
    uint32_t plen,
    const char* key,
    uint32_t keylen,
    JSMN_ALG alg)
{
    uint32_t l;
    const EVP_MD* typ;
    switch (alg) {
        case JSMN_ALG_HS256: typ = EVP_sha256(); break;
        case JSMN_ALG_HS384: typ = EVP_sha384(); break;
        case JSMN_ALG_HS512: typ = EVP_sha512(); break;
        default: return -1;
    }

    HMAC(
        typ,
        key,
        keylen,
        (const unsigned char*)p,
        plen,
        (unsigned char*)dst,
        &l);
    return 0;
}

int
crypto_sign(
    char* dst,
    const char* p,
    uint32_t plen,
    const char* key,
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
