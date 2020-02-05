#ifndef JSMN_WT_H
#define JSMN_WT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct jsmn_web_token_value
    {
        const char* p;
        uint32_t len;
    } jsmn_web_token_value;

#ifdef __cplusplus
}
#endif
#endif
