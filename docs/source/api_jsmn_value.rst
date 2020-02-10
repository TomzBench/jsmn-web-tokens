.. _ref_api_jsmn_value:

jsmn_value
==========

Synopsis
--------

.. code-block:: c

   typedef struct jsmn_value;

Jsmn does not favor NULL terminated strings and instead stores string data as a pointer length tuple. This is because jsmn does not allocate new strings to contain json data but instead collects pointers to the original memory. The jsmn_value tuple is a handle to a JSON token.

Members
-------

1) const char \*p

    Pointer to the start of the JSON value

2) uint32_t len

    Length of the JSON value

Examples
--------

.. rubric:: Read the "sub" key of a jwt token

.. code-block:: c

   const char *jwt = \
      "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."\ 
      "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ."\
      "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";

   jsmn_token_decode_s token;
   err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, jwt, strlen(jwt));

   if (!err) {
     jsmn_value sub;
     jsmn_token_get_claim_str(&token, "sub", &sub);
     printf("Hello %.*s!", sub.len, sub.p);
   }

.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_get_claim_str`
