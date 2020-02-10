.. _ref_api_jsmn_token_get_claim_int:

jsmn_token_get_claim_int
========================

Synopsis
--------

.. code-block:: c

   int jsmn_token_get_claim_int(
       jsmn_token_decode_s* token,
       const char* claim,
       int* result);

After you decode a JWT token with :ref:`ref_api_jsmn_token_decode` you can use this routine to parse the token claims. If the json claim you are parsing is an integer, use :ref:`ref_api_jsmn_token_get_claim_int` and if the json claim you are parsing is a string use :ref:`ref_api_jsmn_token_get_claim_str`.


Parameters
----------

====================================== ===========
Parameter                              Description
====================================== ===========
jsmn_token_decode_s \*token            A context to manage decoder state
const char \*claim                     The name of the claim to parse. (IE: "iat")
int \* result                          The integer value of the json key is returned to the result parameter
====================================== ===========


Examples
--------

**Read the "Subject", "Name", and "Issued at" claims of a JWT**

.. code-block:: c
  
   const char *jwt = \
      "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."\ 
      "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ."\
      "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";

   int err;
   jsmn_token_decode_s token;

   // Decode and validate token
   err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, jwt, strlen(jwt));

   if (!err) {
     jsmn_value sub, name;
     int iat;

     // Read claims
     jsmn_token_get_claim_str(&token, "sub", &sub);
     jsmn_token_get_claim_str(&token, "name", &name);
     jsmn_token_get_claim_int(&token, "iat", &iat);
     printf("Hello %.*s!", name.len, name.p);
   }



.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_decode_s`

2. :ref:`ref_api_jsmn_token_decode`

3. :ref:`ref_api_jsmn_token_get_claim_str`
