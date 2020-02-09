.. _ref_api_jsmn_token_sign:

jsmn_token_sign
===============

Synopsis
--------

.. code-block:: c

   int
   jsmn_token_sign(jsmn_token_encode_s* t, const char* secret, uint32_t slen);

After creating a token with :ref:`ref_api_jsmn_token_init` you can sign the token with :ref:`ref_api_jsmn_token_sign`

Parameters
----------

====================================== ===========
Parameter                              Description
====================================== ===========
jsmn_token_encode_s \*token            A context to manage encoder state
const char \*secret                    A secret of appropriate length per your signing algorithm (aka a key)
uint32_t secret_len                    The length of your signing key
====================================== ===========

Examples
--------

.. code-block:: c

   int err; 
   jsmn_token_encode_s token;

   // Create token
   err = jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));

   if (!err) {
     // Sign token
     err = jsmn_token_sign(&token, "secret", strlen("secret"));


.. rubric:: See Also

1. :ref:`ref_api_jsmn_token_encode_s`

2. :ref:`ref_api_jsmn_token_init`

3. :ref:`ref_api_jsmn_alg`
