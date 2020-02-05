#include "crypto/crypto.h"
#include "jsmn_web_token_private.h"

static const char*
alg_str(JSMN_WEB_TOKEN_ALG alg)
{
    static const char* algs[JSMN_WEB_TOKEN_ALG_COUNT] = { "HS256",
                                                          "HS384",
                                                          "HS512" };

    __jsmn_web_token_assert(alg >= 0 && alg < JSMN_WEB_TOKEN_ALG_COUNT);
    return algs[alg];
}

static inline int
append_b64(jsmn_web_token_s* token, const char* buffer, uint32_t len)
{
    int err;
    uint32_t newlen;
    err = crypto_base64_encode(
        &token->b[token->len],
        sizeof(token->b) - token->len,
        &newlen,
        buffer,
        len);
    if (!err) {
        // base64url encoding w/o padding.
        token->len += newlen;
        if (token->b[token->len - 1] == '=') token->len--;
        if (token->b[token->len - 1] == '=') token->len--;
    }
    return err;
}

int
jsmn_web_token_init(
    jsmn_web_token_s* token,
    JSMN_WEB_TOKEN_ALG alg,
    const char* claims,
    ...)
{
    int err = -1;
    char buffer[JSMN_WEB_TOKEN_MAX_LEN];
    va_list list;

    memset(token, 0, sizeof(jsmn_web_token_s));

    // print the header
    uint32_t dlen, l = snprintf(
                       buffer,
                       sizeof(buffer),
                       "{\"alg\":\"%s\",\"typ\":\"JWT\"}",
                       alg_str(alg));

    // b64(header)
    err = append_b64(token, buffer, l);
    if (err) goto ERROR;
    token->b[token->len++] = '.';

    // b64(header).B64(payload)
    va_start(list, claims);
    l = vsnprintf(buffer, sizeof(buffer), claims, list);
    va_end(list);
    err = append_b64(token, buffer, l);

ERROR:
    return err;
}
