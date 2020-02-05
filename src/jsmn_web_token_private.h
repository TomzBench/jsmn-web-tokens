#ifndef JSMN_WT_PRIVATE_H
#define JSMN_WT_PRIVATE_H

#ifdef linux
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __jsmn_web_token_malloc_fn malloc
#define __jsmn_web_token_free_fn free
#define __jsmn_web_token_assert_fn assert
#define __jsmn_web_token_malloc(x) __jsmn_web_token_malloc_fn(x)
#define __jsmn_web_token_free(x) __jsmn_web_token_free_fn(x)
#define __jsmn_web_token_assert(x) __jsmn_web_token_assert_fn(x)

#elif defined(_WIN32) || defined(WIN32)
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define __jsmn_web_token_malloc_fn malloc
#define __jsmn_web_token_free_fn free
#define __jsmn_web_token_assert_fn assert
#define __jsmn_web_token_malloc(x) __jsmn_web_token_malloc_fn(x)
#define __jsmn_web_token_free(x) __jsmn_web_token_free_fn(x)
#define __jsmn_web_token_assert(x) __jsmn_web_token_assert_fn(x)

#endif

#include "jsmn_web_token/jsmn_web_token.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct jsmn_web_token_s
    {
        JSMN_WEB_TOKEN_ALG alg;
        char json[];
    } jsmn_web_token_s;

#ifdef __cplusplus
}
#endif
#endif
