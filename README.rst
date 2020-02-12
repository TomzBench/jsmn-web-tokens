.. |br| raw:: html

   <br />

Jasmine Web Tokens
==================

.. image:: https://travis-ci.com/TomzBench/jsmn-web-tokens.svg?branch=master
    :target: https://travis-ci.com/TomzBench/jsmn-web-tokens
.. image:: https://codecov.io/gh/TomzBench/jsmn-web-tokens/branch/master/graph/badge.svg
    :target: https://codecov.io/gh/TomzBench/jsmn-web-tokens

|br|

.. image:: docs/source/_static/img/view-on-jwt.io.svg
    :target: https://jwt.io

About
-----

Small library for creating and decoding `Json Web Tokens <https://jwt.io>`_

Docs
----

https://tomzbench.github.io/jsmn-web-tokens

Example
-------

**Create some token**

.. code-block:: c
  
  int err; 
  jsmn_token_encode_s token;

  // Create token
  err = jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));

  if (!err) {
    // Sign token
    err = jsmn_token_sign(&token, "secret", strlen("secret"));

    // Get token
    if (!err) printf("%.*s", jsmn_token_len(&token), jsmn_token_str(&token));
  }


**Decode some token**

.. code-block:: c
  
  const char *t = \
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."\ 
    "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ."\
    "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";

  int err;
  jsmn_token_decode_s token;

  // Decode and validate token
  err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, t, strlen(t));

  if (!err) {
    jsmn_value sub, name;
    int iat;

    // Read claims
    jsmn_token_get_claim_str(&token, "sub", &sub);
    jsmn_token_get_claim_str(&token, "name", &name);
    jsmn_token_get_claim_int(&token, "iat", &iat);
    printf("Hello %.*s!", name.len, name.p);
  }
