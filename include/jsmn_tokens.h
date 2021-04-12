#ifndef JSMN_WT_H
#define JSMN_WT_H

#include <stdbool.h>
#include <stdint.h>

// clang-format off
#if defined _WIN32
#  include <assert.h>
#  include <stdarg.h>
#  include <stdbool.h>
#  include <stdint.h>
#  include <stdio.h>
#  include <string.h>
#  define __jsmn_malloc_fn malloc
#  define __jsmn_free_fn free
#  define __jsmn_assert_fn assert
#  define __jsmn_malloc(x) __jsmn_malloc_fn(x)
#  define __jsmn_free(x) __jsmn_free_fn(x)
#  define __jsmn_assert(x) __jsmn_assert_fn(x)
#  if defined JSMN_TOKENS_STATIC
#    define JSMN_TOKENS_API
#  elif defined DLL_EXPORT
#    define JSMN_TOKENS_API __declspec(dllexport)
#  else
#    define JSMN_TOKENS_API __declspec(dllimport)
#  endif
#else
#  include <assert.h>
#  include <stdarg.h>
#  include <stdbool.h>
#  include <stddef.h>
#  include <stdint.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  define __jsmn_malloc_fn malloc
#  define __jsmn_free_fn free
#  define __jsmn_assert_fn assert
#  define __jsmn_malloc(x) __jsmn_malloc_fn(x)
#  define __jsmn_free(x) __jsmn_free_fn(x)
#  define __jsmn_assert(x) __jsmn_assert_fn(x)
#  define JSMN_TOKENS_API
#endif
// clang-format on

#ifndef JSMN_MAX_TOKEN_LEN
#define JSMN_MAX_TOKEN_LEN 256
#endif

#ifndef JSMN_MAX_TOKEN_HEADER_LEN
#define JSMN_MAX_TOKEN_HEADER_LEN 64
#endif

#ifndef JSMN_MAX_TOKENS
#define JSMN_MAX_TOKENS 64
#endif

#ifndef JSMN_MAX_HEADER_TOKENS
#define JSMN_MAX_HEADER_TOKENS 16
#endif

#define JSMN_HEADER
#include "jsmn.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned char byte;

    typedef struct jsmn_value
    {
        const char* p;
        uint32_t len;
    } jsmn_value;

    typedef enum JSMN_ALG
    {
        JSMN_ALG_NONE = -1,
        JSMN_ALG_HS256 = 0,
        JSMN_ALG_HS384 = 1,
        JSMN_ALG_HS512 = 2,
        // JSMN_ALG_RS256,
        // JSMN_ALG_RS384,
        // JSMN_ALG_RS512,
        // JSMN_ALG_ES256,
        // JSMN_ALG_ES384,
        // JSMN_ALG_ES512,
        // JSMN_ALG_TERM
    } JSMN_ALG;
#define JSMN_ALG_COUNT 3

    typedef enum JSMN_ERROR
    {
        JSMN_ERROR_OK = 0
    } JSMN_ERROR;

    typedef struct jsmn_token_encode_s
    {
        JSMN_ALG alg;
        uint32_t len;
        char b[JSMN_MAX_TOKEN_LEN];
    } jsmn_token_encode_s;

    typedef struct jsmn_token_decoded_s
    {
        JSMN_ALG alg;
        jsmntok_t head[JSMN_MAX_HEADER_TOKENS];
        jsmntok_t body[JSMN_MAX_TOKENS];
        uint32_t n_head;
        uint32_t n_body;
        uint32_t json_len;
        char json[JSMN_MAX_TOKEN_LEN];
    } jsmn_token_decode_s;

    JSMN_TOKENS_API int jsmn_token_init(
        jsmn_token_encode_s* t,
        JSMN_ALG alg,
        const char* claims,
        ...);
    JSMN_TOKENS_API int
    jsmn_token_sign(jsmn_token_encode_s* t, const char* secret, uint32_t slen);
    JSMN_TOKENS_API uint32_t jsmn_token_len(jsmn_token_encode_s* t);
    JSMN_TOKENS_API const char* jsmn_token_str(jsmn_token_encode_s* t);
    JSMN_TOKENS_API int jsmn_token_decode(
        jsmn_token_decode_s*,
        const char* secret,
        JSMN_ALG,
        const char*,
        uint32_t);
    JSMN_TOKENS_API int jsmn_token_get_claim_str(
        jsmn_token_decode_s* t,
        const char* claim,
        jsmn_value* result);
    JSMN_TOKENS_API int jsmn_token_get_claim_int(
        jsmn_token_decode_s* token,
        const char* claim,
        int* result);

    // TODO deprecate jsmn_token_data
#define jsmn_token_data(x) jsmn_token_str(x)

#ifdef __cplusplus
}
#endif
#endif
