#ifndef JSMN_HELPERS_H
#define JSMN_HELPERS_H

#include "jsmn/jsmn_tokens.h"
#include "jsmn_tokens_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define JSMN_HEADER
#include "jsmn/jsmn.h"

#define jsmn_helper(x) jsmntok_t[x]

    void jsmn_foreach(
        jsmntok_t* t,
        uint32_t n_tokens,
        const char* data,
        void (*cb)(void*, jsmn_value* key, jsmn_value* val),
        void*);

    uint32_t jsmn_parse_tokens(
        jsmntok_t*,
        uint32_t sz,
        const char* data,
        uint32_t,
        uint32_t,
        ...);

    uint32_t jsmn_parse_tokens_path(
        const char* path,
        jsmntok_t*,
        uint32_t sz,
        const char* data,
        uint32_t,
        uint32_t,
        ...);

#ifdef __cplusplus
}
#endif
#endif
