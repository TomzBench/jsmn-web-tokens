#ifndef CRYPTO_H
#define CRYPTO_H

#if defined(WITH_OPENSSL)
#include "crypto_openssl.h"
#elif defined(WITH_GNUTLS)
#elif defined(WITH_MBEDTLS)
#include "crypto_mbedtls.h"
#elif defined(WITH_WOLFSSL)
#endif

#include "base64uri.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
#endif
