.. _ref_api_jsmn_token_decode:

jsmn_token_decode
=================

Synopsis
--------

.. code-block:: c

   int jsmn_token_decode(
       jsmn_token_decode_s*,
       const char* secret,
       JSMN_ALG,
       const char*,
       uint32_t);

Convert a JSON Web Token in dot notation format into JSON data. Internally this routine will check to make sure the JWT header algorithm matches the algorithm argument. It will also verify the signature with callers secret key.

Parameters
----------

====================================== ===========
Parameter                              Description
====================================== ===========
jsmn_token_decode_s \*token            A context to manage decoder state
const char \*secret                    A key of appropriate length per your signing algorithm
JSMN_ALG alg                           An algorithm for to sign token with
const char \*jwt                       JWT string in dot notation format
uint32_t len                           Length of JWT string
====================================== ===========


Examples
--------

**Decode some token**

.. code-block:: c
  
   const char *jwt = \
   eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.\ 
   eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.\
   SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c;

   int err;
   jsmn_token_decode_s token;

   // Decode and validate token
   err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, jwt, strlen(jwt));

.. rubric:: See Also

1. :ref:`ref_api_jsmn_alg`
