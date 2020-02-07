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

#include "jsmn/jsmn_tokens.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned char byte;

#ifdef __cplusplus
}
#endif
#endif
