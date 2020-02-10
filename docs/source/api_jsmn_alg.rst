.. _ref_api_jsmn_alg:

JSMN_ALG
========

Synopsis
--------

.. code-block:: c
    
    typedef enum JSMN_ALG;

jsmn-web-tokens API requires a specific algorithm to be passed to identify how the function should encode or decode the web token.

Members
-------

* JSMN_ALG_HS256

* JSMN_ALG_HS384

* JSMN_ALG_HS512

Algorithms Supported
--------------------

=========  ======= ======= ======= ======
Algorithm  MBEDTLS WOLFSSL OPENSSL GNUTLS
=========  ======= ======= ======= ======
HS256      YES     NO      YES     NO
HS384      YES     NO      YES     NO
HS512      YES     NO      YES     NO
=========  ======= ======= ======= ======

Examples
--------

.. rubric:: Decode a HS256 token

.. code-block:: c

   const char *jwt = \
   eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.\ 
   eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.\
   SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c;

   jsmn_token_decode_s token;
   err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, jwt, strlen(jwt));


.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_init`

2. :ref:`ref_api_jsmn_token_decode`
