#ifndef JSMN_HELPERS_H
#define JSMN_HELPERS_H

#include "jsmn/jsmn_tokens.h"

#ifdef linux
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __jsmn_malloc_fn malloc
#define __jsmn_free_fn free
#define __jsmn_assert_fn assert
#define __jsmn_malloc(x) __jsmn_malloc_fn(x)
#define __jsmn_free(x) __jsmn_free_fn(x)
#define __jsmn_assert(x) __jsmn_assert_fn(x)

#elif defined(_WIN32) || defined(WIN32)
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define __jsmn_malloc_fn malloc
#define __jsmn_free_fn free
#define __jsmn_assert_fn assert
#define __jsmn_malloc(x) __jsmn_malloc_fn(x)
#define __jsmn_free(x) __jsmn_free_fn(x)
#define __jsmn_assert(x) __jsmn_assert_fn(x)

#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define JSMN_HEADER
#include "jsmn/jsmn.h"

#define jsmn_helper(x) jsmntok_t[x]

    typedef unsigned char byte;

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

// TODO not in use until we figure out how to pass code into a macro?
#define __jsmn_foreach(t, n, str, code)                                        \
    do {                                                                       \
        jsmn_value val, key;                                                   \
        uint32_t __end, __i = 0;                                               \
        bool __expect_key = true;                                              \
        if (t[__i].type == JSMN_OBJECT || t[__i].type == JSMN_ARRAY) __i++;    \
        while (__i < n) {                                                      \
            if (t[__i].type == JSMN_OBJECT || t[__i].type == JSMN_ARRAY) {     \
                __end = t[__i].end;                                            \
                while (__i < n && t[__i].start < __end) __i++;                 \
                __expect_key = true;                                           \
                __i++;                                                         \
                continue;                                                      \
            }                                                                  \
            if (__expect_key) {                                                \
                __expect_key = false;                                          \
                key.p = &data[t[__i].start];                                   \
                key.len = t[__i].end - t[__i].start;                           \
            } else {                                                           \
                __expect_key = true;                                           \
                val.p = &data[t[__i].start];                                   \
                val.len = t[__i].end - t[__i].start;                           \
                code                                                           \
            }                                                                  \
            __i++;                                                             \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif
#endif
