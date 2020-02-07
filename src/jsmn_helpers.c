#include "jsmn/jsmn.h"

#include "jsmn_helpers.h"

#define __seek_slash(__bytes, __len, __result)                                 \
    do {                                                                       \
        while (__result < __len &&                                             \
               !(__bytes[__result] == '/' || __bytes[__result] == '\\'))       \
            __result++;                                                        \
                                                                               \
    } while (0)

#define __skip_object(__token, __n, __spot)                                    \
    do {                                                                       \
        int __end = __token[__spot].end;                                       \
        while (__spot < __n && __token[__spot].start < __end) __spot++;        \
    } while (0)

static bool
is_object(jsmntok_t* t)
{
    return t->type == JSMN_OBJECT || t->type == JSMN_ARRAY;
}

static void
get_token(jsmntok_t* t, const char* data, const char** str_p, uint32_t* len)
{
    *str_p = &data[t->start];
    *len = t->end - t->start;
}

static bool
map_key_to_value(
    const char* key,
    uint32_t keylen,
    const char* value,
    uint32_t value_len,
    uint32_t n_tags,
    va_list list)
{
    uint32_t c = n_tags << 1;
    char* cmp;
    while (c >= 2) {
        c -= 2;
        cmp = va_arg(list, char*);
        jsmn_value* result = va_arg(list, jsmn_value*);
        if (keylen == strlen(cmp) && !memcmp(key, cmp, keylen)) {
            result->len = value_len;
            result->p = value;
            return true;
        }
    }
    return false;
}

void
jsmn_foreach(
    jsmntok_t* t,
    uint32_t n_tokens,
    const char* data,
    void (*cb)(void*, jsmn_value* key, jsmn_value* val),
    void* ctx)
{
    uint32_t i = 0;
    jsmn_value key, val;
    bool expect_key = true;
    if (is_object(&t[i])) i++;
    while (i < n_tokens) {
        if (is_object(&t[i])) {
            __skip_object(t, n_tokens, i);
            expect_key = true;
            continue;
        }
        if (expect_key) {
            get_token(&t[i], data, &key.p, &key.len);
            expect_key = false;
        } else {
            get_token(&t[i], data, &val.p, &val.len);
            expect_key = true;
            cb(ctx, &key, &val);
        }
        i++;
    }
}

uint32_t
jsmn_parse_tokens(
    jsmntok_t* t,
    uint32_t max_tokens,
    const char* data,
    uint32_t sz,
    uint32_t n_tags,
    ...)
{
    uint32_t n_tokens, i = 0;
    jsmn_value tag = { .p = NULL, .len = 0 };
    jsmn_parser p;
    jsmn_init(&p);
    bool expect_key = true;
    uint32_t count = 0;
    n_tokens = jsmn_parse(&p, data, sz, t, max_tokens);
    if (!(n_tokens <= max_tokens)) return 0;
    if (is_object(&t[i])) i++;
    while (i < n_tokens) {
        if (is_object(&t[i])) {
            __skip_object(t, n_tokens, i);
            expect_key = true;
            continue;
        }
        if (expect_key) {
            expect_key = false;
            get_token(&t[i], data, &tag.p, &tag.len);
        } else {
            expect_key = true;
            va_list list;
            va_start(list, n_tags);
            count += map_key_to_value(
                         tag.p,
                         tag.len,
                         &data[t[i].start],
                         t[i].end - t[i].start,
                         n_tags,
                         list)
                         ? 1
                         : 0;
            va_end(list);
        }
        i++;
    }
    return count;
}

uint32_t
jsmn_parse_tokens_path(
    const char* path,
    jsmntok_t* t,
    uint32_t max_tokens,
    const char* data,
    uint32_t sz,
    uint32_t n_tags,
    ...)
{
    uint32_t n_tokens, count = 0;
    jsmn_parser p;
    jsmn_init(&p);
    n_tokens = jsmn_parse(&p, data, sz, t, max_tokens);
    if (!(n_tokens <= max_tokens)) return count;

    uint32_t parent = 0, i = 0, spot = 0, cmplen;

    while (i < n_tokens) {
        if (is_object(&t[i])) {
            (parent = i, i++);
        }
        if (t[i].type == JSMN_STRING) {
            if (*path == '/' || *path == '\\') path++;
            __seek_slash(path, strlen(path), spot);
            cmplen = t[i].end - t[i].start;
            if (spot == cmplen && !memcmp(path, &data[t[i].start], spot)) {
                path += spot;
                i++;
                parent = i;
                if (path && !*path) {
                    // Found the object we are looking for...
                    const char* tag = NULL;
                    uint32_t taglen = 0;
                    bool expect_key = true;
                    if (is_object(&t[i])) i++;
                    while (i < n_tokens && t[i].end <= t[parent].end) {
                        if (is_object(&t[i])) {
                            __skip_object(t, n_tokens, i);
                            expect_key = true;
                            continue;
                        }
                        if (expect_key) {
                            expect_key = false;
                            get_token(&t[i], data, &tag, &taglen);
                        } else {
                            expect_key = true;
                            va_list list;
                            va_start(list, n_tags);
                            count += map_key_to_value(
                                         tag,
                                         taglen,
                                         &data[t[i].start],
                                         t[i].end - t[i].start,
                                         n_tags,
                                         list)
                                         ? 1
                                         : 0;
                            va_end(list);
                        }
                        i++;
                    }
                    break;
                }
            } else {
                __skip_object(t, n_tokens, i);
            }
        } else {
            // JSMN_UNDEFINED || JSMN_ARRAY || JSMN_PRIMITIVE ?
            i++;
        }
    }
    return count;
}
