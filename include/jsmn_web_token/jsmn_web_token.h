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

    typedef enum JSMN_WEB_TOKEN_ALG
    {
        JWT_ALG_NONE = 0,
        JWT_ALG_HS256,
        JWT_ALG_HS384,
        JWT_ALG_HS512,
        JWT_ALG_RS256,
        JWT_ALG_RS384,
        JWT_ALG_RS512,
        JWT_ALG_ES256,
        JWT_ALG_ES384,
        JWT_ALG_ES512,
        JWT_ALG_TERM
    } JSMN_WEB_TOKEN_ALG;

#ifdef __cplusplus
}
#endif
#endif
