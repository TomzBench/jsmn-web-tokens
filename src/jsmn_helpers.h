#ifndef JSMN_HELPERS_H
#define JSMN_HELPERS_H

#include "jsmn_wt/jsmn_wt.h"
#include "jsmn_wt_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define JSMN_HEADER
#include "jsmn.h"

#define jsmn_helper(x) jsmntok_t[x]

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
