#include "base64uri.h"
#include "base64.h"

static inline uint32_t
calc_max_b64_estimate(uint32_t sz)
{
    uint32_t half = sz >> 1;
    return sz + half + 2;
}

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
    if (dst_len < (calc_max_b64_estimate(src_len))) return -1;
    *out_len = crypto_b64encode(dst, src, src_len) - 1;
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
    int err;
    char b[JSMN_MAX_TOKEN_LEN];
    uint32_t l, blen = sizeof(b);
    if (dlen < (calc_max_b64_estimate(slen))) return -1;
    err = b64uri_to_b64(b, &blen, src, slen);
    if (err) goto ERROR;
    b[blen] = 0;
    l = crypto_b64decode(dst, b);
    *olen = strlen(dst);       // TODO investigate b64_encode return length
    __jsmn_assert(*olen == l); // (testing if we can use return)
ERROR:
    return err;
}

