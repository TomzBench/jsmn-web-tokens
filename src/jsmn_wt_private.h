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
#define __jsmn_wt_malloc_fn malloc
#define __jsmn_wt_free_fn free
#define __jsmn_wt_assert_fn assert
#define __jsmn_wt_malloc(x) __jsmn_wt_malloc_fn(x)
#define __jsmn_wt_free(x) __jsmn_wt_free_fn(x)
#define __jsmn_wt_assert(x) __jsmn_wt_assert_fn(x)

#elif defined(_WIN32) || defined(WIN32)
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define __jsmn_wt_malloc_fn malloc
#define __jsmn_wt_free_fn free
#define __jsmn_wt_assert_fn assert
#define __jsmn_wt_malloc(x) __jsmn_wt_malloc_fn(x)
#define __jsmn_wt_free(x) __jsmn_wt_free_fn(x)
#define __jsmn_wt_assert(x) __jsmn_wt_assert_fn(x)

#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
#endif
