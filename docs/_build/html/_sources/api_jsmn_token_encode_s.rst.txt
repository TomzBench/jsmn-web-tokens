.. _ref_api_jsmn_token_encode_s:

jsmn_token_encode_s
===================

Synopsis
--------

.. code-block:: c

   typedef struct jsmn_token_encode_s;

An "encoder" context used to encode a new JSON web token. Initialize a jsmn_token_encode_s context with :ref:`ref_api_jsmn_token_init`.

Examples
--------

.. code-block:: c

   jsmn_token_encode_s token;
 
   // Create token
   err = jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));
 

.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_init`
