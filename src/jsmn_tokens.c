#include "crypto/crypto.h"
#include "jsmn_helpers.h"
#include "jsmn_tokens_private.h"

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
    JSMN_ALG alg = JSMN_ALG_ERROR;
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
        case JSMN_ALG_ERROR: return 0; break;
        case JSMN_ALG_HS256: return 32; break;
        case JSMN_ALG_HS384: return 48; break;
        case JSMN_ALG_HS512: return 64; break;
    }
}

static inline int
append_b64(jsmn_token_s* token, const char* buffer, uint32_t len)
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
append_dot(jsmn_token_s* token)
{
    __jsmn_assert(token->len < sizeof(token->b));
    token->b[token->len++] = '.';
}

int
jsmn_token_init(jsmn_token_s* token, JSMN_ALG alg, const char* claims, ...)
{
    int err = -1;
    char buffer[JSMN_MAX_TOKEN_LEN];
    va_list list;

    memset(token, 0, sizeof(jsmn_token_s));
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

int
jsmn_token_sign(jsmn_token_s* t, const char* key, uint32_t keylen)
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

uint32_t
jsmn_token_len(jsmn_token_s* t)
{
    return t->len;
}

const char*
jsmn_token_data(jsmn_token_s* t)
{
    return t->b;
}

int
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
    jsmn_value head, body, sig, alg, typ;
    jsmn_parser p;

    memset(t, 0, sizeof(jsmn_token_decode_s));

    // populate head
    head.p = token;
    dot = memchr(token, '.', token_len);
    if (!dot) goto ERROR;
    head.len = dot - head.p;

    // populate body
    body.p = ++dot;
    dot = memchr(dot, '.', &token[token_len] - dot);
    if (!dot) goto ERROR;
    body.len = dot - body.p;

    // populate sig
    sig.p = ++dot;
    sig.len = token_len - head.len - body.len - 2;

    err = crypto_base64uri_decode(b, sizeof(b), &l, head.p, head.len);
    if (err) goto ERROR;

    // clang-format off
    t->n_head = jsmn_parse_tokens(
        t->head,
        JSMN_MAX_HEADER_TOKENS,
        b,
        l,
        2,
        "alg", &alg,
        "typ", &typ);
    // clang-format on
    t->alg = str_to_alg(alg.p, alg.len);

    err = -1;
    if (!(t->n_head >= 2)) goto ERROR;
    if (!(typ.len == 3)) goto ERROR;
    if (!(t->alg == use_alg)) goto ERROR;
    if (memcmp(typ.p, "JWT", typ.len)) goto ERROR;

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

typedef struct seek_claim_context
{
    jsmn_value* find;
    const char* claim;
    uint32_t claim_len;
} seek_claim_context;

static void
seek_claim(void* context, jsmn_value* key, jsmn_value* val)
{
    seek_claim_context* ctx = context;
    if (key->len == ctx->claim_len && !(memcmp(key->p, ctx->claim, key->len))) {
        ctx->find->p = val->p;
        ctx->find->len = val->len;
    }
}

int
jsmn_token_get_claim_str(
    jsmn_token_decode_s* token,
    const char* claim,
    jsmn_value* result)
{
    int ret = -1;
    jsmn_value find = { .p = NULL, .len = 0 };
    seek_claim_context ctx = { .find = &find,
                               .claim = claim,
                               .claim_len = strlen(claim) };
    result->p = NULL;
    result->len = 0;
    jsmn_foreach(token->body, token->n_body, token->json, seek_claim, &ctx);
    if (find.p) {
        result->p = find.p;
        result->len = find.len;
        ret = 0;
    }
    return ret;
}

int
jsmn_token_get_claim_int(
    jsmn_token_decode_s* token,
    const char* claim,
    int* result)
{
    int ret = -1;
    char buff[32];
    jsmn_value find = { .p = NULL, .len = 0 };
    seek_claim_context ctx = { .find = &find,
                               .claim = claim,
                               .claim_len = strlen(claim) };
    *result = 0;
    jsmn_foreach(token->body, token->n_body, token->json, seek_claim, &ctx);
    if (find.p) {
        snprintf(buff, sizeof(buff), "%.*s", find.len, find.p);
        *result = atoi(buff);
        ret = 0;
    }
    return ret;
}
