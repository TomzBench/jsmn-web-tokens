.. _ref_api_jsmn_token_init:

jsmn_token_init
===============

Synopsis
--------

.. code-block:: c

   int jsmn_token_init(
       jsmn_token_encode_s* t,
       JSMN_ALG alg,
       const char* claims,
       ...);
   
Convert JSON data into a signed encoded JWT object in dot notation format. Internally this method will populate a JSON header ``"{\"alg\":\"%s\",\"typ\":\"JWT\"}"`` where ``%s`` is determined by the algorithm argument. Then, a string of JSON "claims" is prepended to the header.

.. warning:: Your "claims" is expected to be a valid JSON string, and validation is not performed for the caller. Caller should treat the last two arguments as common for typical printf or snprintf family of functions. Internally this routine will use snprintf to build your JSON payload. If the result of your snprintf routine is not a valid JSON string then you will have a properly encoded and signed token that will not parse! (AKA a junky token) =P. This is not a hand holding library but a tiny library for constrained enviorments.

Parameters
----------

====================================== ===========
Parameter                              Description
====================================== ===========
jsmn_token_encode_s \*token            A context to manage encoder state
JSMN_ALG alg                           An algorithm for to sign token with
const char \*claims format string      JSON encoded format string
Arguments to format string             variadic arguments to the format string
====================================== ===========

Examples
--------

.. code-block:: c

   int err; 
   jsmn_token_encode_s token;

   // Create token
   err = jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));

.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_encode_s`

2. :ref:`ref_api_jsmn_alg`
