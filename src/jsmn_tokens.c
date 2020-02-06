#include "crypto/crypto.h"
#include "jsmn_tokens_private.h"

static const char*
alg_str(JSMN_ALG alg)
{
    static const char* algs[JSMN_ALG_COUNT] = { "HS256", "HS384", "HS512" };

    __jsmn_assert(alg >= 0 && alg < JSMN_ALG_COUNT);
    return algs[alg];
}

static inline int
append_b64(jsmn_token_s* token, const char* buffer, uint32_t len)
{
    int err;
    uint32_t newlen;
    err = crypto_base64_encode(
        &token->b[token->len],
        sizeof(token->b) - token->len,
        &newlen,
        buffer,
        len);
    if (!err) token->len += newlen;
    return err;
}

static inline void
append_dot(jsmn_token_s* token)
{
    __jsmn_assert(token->len < sizeof(token->b));
    token->b[token->len++] = '.';
}

int
jsmn_token_init(jsmn_token_s* token, JSMN_ALG alg, const char* claims, ...)
{
    int err = -1;
    char buffer[JSMN_MAX_LEN];
    va_list list;

    memset(token, 0, sizeof(jsmn_token_s));
    token->alg = alg;

    // print the header
    uint32_t dlen, l = snprintf(
                       buffer,
                       sizeof(buffer),
                       "{\"alg\":\"%s\",\"typ\":\"JWT\"}",
                       alg_str(alg));

    // b64(header)
    err = append_b64(token, buffer, l);
    if (err) goto ERROR;

    // b64(headar) .
    append_dot(token);

    // b64(header) . B64(payload)
    va_start(list, claims);
    l = vsnprintf(buffer, sizeof(buffer), claims, list);
    va_end(list);
    err = append_b64(token, buffer, l);

ERROR:
    return err;
}

int
jsmn_token_sign(jsmn_token_s* t, const char* key, uint32_t keylen)
{
    char hash[32] = { 0 };
    char bhash[64] = { 0 };
    int err;
    uint32_t l = 0;

    err = crypto_sign(hash, t->b, t->len, (byte*)key, keylen, t->alg);
    if (err) goto ERROR;

    append_dot(t);

    err = append_b64(t, hash, sizeof(hash));
    if (err) goto ERROR;

ERROR:
    return err;
}

int
jsmn_token_decode(jsmn_token_s* t, const char* token, uint32_t token_len)
{
    return -1;
}
