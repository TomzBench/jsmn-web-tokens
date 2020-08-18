#include "jsmn.h"

#include "crypto/crypto.h"
#include "jsmn_tokens.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define json_for_each_obj(i, t, obj)                                           \
    for (i = 0, t = (obj) + 1; i < (obj)->size; t = json_next(t + 1), i++)
#define json_for_each_arr(i, t, arr)                                           \
    for (i = 0, t = (arr) + 1; i < (arr)->size; t = json_next(t), i++)

static const jsmntok_t*
json_next(const jsmntok_t* tok)
{
    const jsmntok_t* t;
    size_t i;

    for (t = tok + 1, i = 0; i < tok->size; i++) t = json_next(t);

    return t;
}

static bool
json_tok_streq(const char* buffer, const jsmntok_t* tok, const char* str)
{
    if (tok->type != JSMN_STRING) return false;
    if (tok->end - tok->start != strlen(str)) return false;
    return strncmp(buffer + tok->start, str, tok->end - tok->start) == 0;
}

static const jsmntok_t*
json_get_member(const char* buffer, const jsmntok_t tok[], const char* label)
{
    const jsmntok_t* t;
    size_t i;

    if (tok->type != JSMN_OBJECT) return NULL;

    json_for_each_obj(
        i, t, tok) if (json_tok_streq(buffer, t, label)) return t +
        1;

    return NULL;
}

static const char* alg_strings[JSMN_ALG_COUNT] = { "HS256", "HS384", "HS512" };

static const char*
alg_to_str(JSMN_ALG alg)
{

    __jsmn_assert(alg >= 0 && alg < JSMN_ALG_COUNT);
    return alg_strings[alg];
}

static JSMN_ALG
str_to_alg(const char* str, uint32_t len)
{
    JSMN_ALG alg = JSMN_ALG_NONE;
    if (len == 5) {
        for (int i = 0; i < JSMN_ALG_COUNT; i++) {
            if (!memcmp(str, alg_strings[i], 5)) {
                alg = i;
                break;
            }
        }
    }
    return alg;
}

static uint32_t
alg_to_keysize(JSMN_ALG alg)
{
    switch (alg) {
        case JSMN_ALG_NONE: return 0; break;
        case JSMN_ALG_HS256: return 32; break;
        case JSMN_ALG_HS384: return 48; break;
        case JSMN_ALG_HS512: return 64; break;
    }
}

static inline int
append_b64(jsmn_token_encode_s* token, const char* buffer, uint32_t len)
{
    int err;
    uint32_t newlen;
    err = crypto_base64uri_encode(
        &token->b[token->len],
        sizeof(token->b) - token->len,
        &newlen,
        buffer,
        len);
    if (!err) token->len += newlen;
    return err;
}

static inline void
append_dot(jsmn_token_encode_s* token)
{
    __jsmn_assert(token->len < sizeof(token->b));
    token->b[token->len++] = '.';
}

JSMN_TOKENS_API int
jsmn_token_init(
    jsmn_token_encode_s* token,
    JSMN_ALG alg,
    const char* claims,
    ...)
{
    int err = -1;
    char buffer[JSMN_MAX_TOKEN_LEN];
    va_list list;

    memset(token, 0, sizeof(jsmn_token_encode_s));
    token->alg = alg;

    // print the header
    uint32_t dlen, l = snprintf(
                       buffer,
                       sizeof(buffer),
                       "{\"alg\":\"%s\",\"typ\":\"JWT\"}",
                       alg_to_str(alg));

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

JSMN_TOKENS_API int
jsmn_token_sign(jsmn_token_encode_s* t, const char* key, uint32_t keylen)
{
    char hash[512] = { 0 };
    int err;
    uint32_t l = 0;

    err = crypto_sign(hash, t->b, t->len, (byte*)key, keylen, t->alg);
    if (err) goto ERROR;

    append_dot(t);

    err = append_b64(t, hash, alg_to_keysize(t->alg));
    if (err) goto ERROR;

ERROR:
    return err;
}

JSMN_TOKENS_API uint32_t
jsmn_token_len(jsmn_token_encode_s* t)
{
    return t->len;
}

JSMN_TOKENS_API const char*
jsmn_token_data(jsmn_token_encode_s* t)
{
    return t->b;
}

JSMN_TOKENS_API int
jsmn_token_decode(
    jsmn_token_decode_s* t,
    const char* secret,
    JSMN_ALG use_alg,
    const char* token,
    uint32_t token_len)
{
    char b[JSMN_MAX_TOKEN_HEADER_LEN];
    const char* dot;
    int err = -1;
    uint32_t l, slen = secret ? strlen(secret) : 0;
    jsmn_value head, body, sig;
    const jsmntok_t *alg, *typ;
    jsmn_parser p;

    memset(t, 0, sizeof(jsmn_token_decode_s));

    // populate head
    head.p = token;
    dot = memchr(token, '.', token_len);
    if (!(dot && (head.len = dot - head.p))) goto ERROR;

    // populate body
    body.p = ++dot;
    dot = memchr(dot, '.', &token[token_len] - dot);
    if (!(dot && (body.len = dot - body.p))) goto ERROR;

    // populate sig
    sig.p = ++dot;
    if (!(sig.len = token_len - head.len - body.len - 2)) goto ERROR;

    err = crypto_base64uri_decode(b, sizeof(b), &l, head.p, head.len);
    if (err) goto ERROR;

    jsmn_init(&p);
    err = jsmn_parse(&p, b, l, t->head, JSMN_MAX_HEADER_TOKENS);
    if (!(err < JSMN_MAX_HEADER_TOKENS)) goto ERROR;
    alg = json_get_member(b, t->head, "alg");
    typ = json_get_member(b, t->head, "typ");
    if (!(alg && typ &&                   //
          (typ->end - typ->start == 3) && //
          !memcmp(&b[typ->start], "JWT", 3))) {
        err = -1;
        goto ERROR;
    }
    t->alg = str_to_alg(&b[alg->start], alg->end - alg->start);
    if (!(t->alg == use_alg)) {
        err = -1;
        goto ERROR;
    }

    err = crypto_base64uri_decode(
        t->json, sizeof(t->json), &t->json_len, body.p, body.len);
    if (err) goto ERROR;

    jsmn_init(&p);
    t->n_body = jsmn_parse(&p, t->json, t->json_len, t->body, JSMN_MAX_TOKENS);
    if (!t->n_body) goto ERROR;

    err = 0;

    if (!err && secret) {
        char hash[512] = { 0 };
        char test_sig[1024];
        l = sizeof(test_sig);
        err = crypto_sign(
            hash, head.p, head.len + body.len + 1, (byte*)secret, slen, t->alg);
        if (err) goto ERROR;
        err = crypto_base64uri_encode(
            test_sig, l, &l, hash, alg_to_keysize(t->alg));
        if (err) goto ERROR;
        err = sig.len == l && !memcmp(test_sig, sig.p, sig.len) ? 0 : -1;
    }

ERROR:
    return err;
}

JSMN_TOKENS_API int
jsmn_token_get_claim_str(
    jsmn_token_decode_s* token,
    const char* claim,
    jsmn_value* result)
{
    const jsmntok_t* find = json_get_member(token->json, token->body, claim);
    if (find) {
        result->p = &token->json[find->start];
        result->len = find->end - find->start;
        return 0;
    } else {
        return -1;
    }
}

JSMN_TOKENS_API int
jsmn_token_get_claim_int(
    jsmn_token_decode_s* token,
    const char* claim,
    int* result)
{
    char buff[32];
    const jsmntok_t* find = json_get_member(token->json, token->body, claim);
    if (find) {
        snprintf(
            buff,
            sizeof(buff),
            "%.*s",
            find->end - find->start,
            &token->json[find->start]);
        *result = atoi(buff);
        return 0;
    } else {
        return -1;
    }
}

