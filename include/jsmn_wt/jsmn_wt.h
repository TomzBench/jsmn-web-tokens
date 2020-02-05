#ifndef JSMN_WT_H
#define JSMN_WT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct jsmn_wt_str
    {
        const char* p;
        uint32_t len;
    } jsmn_value;

#ifdef __cplusplus
}
#endif
#endif
