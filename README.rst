jsmn-web-tokens
===============

.. image:: https://travis-ci.com/TomzBench/jsmn_web_tokens.svg?branch=master
    :target: https://travis-ci.com/TomzBench/jsmn_web_tokens
.. image:: https://codecov.io/gh/TomzBench/jsmn_web_tokens/branch/master/graph/badge.svg
  :target: https://codecov.io/gh/TomzBench/jsmn_web_tokens

About
-----

Small library for creating and decoding `Json Web Tokens<https://jwt.io>`_.

Docs
----

https://tomzbench.github.io/jsmn-web-tokens

Example
-------

**Create somee token**

.. code-block:: c
  
  int err, jsmn_token_s token;

  err = jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));

  if (!err) printf("%.*s", jsmn_token_len(&token), jsmn_token(&token));

**Decode some token**

.. code-block:: c
  
  const char *token = \
  eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.\ 
  eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.\
  SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c;

  int err;
  jsmn_token_decode_s token;

  err = jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, t, strlen(t));

  if (!err) {
    jsmn_value sub, name;
    int iat;
    jsmn_token_get_claim_str(&token, "sub", &sub);
    jsmn_token_get_claim_str(&token, "name", &name);
    jsmn_token_get_claim_int(&token, "iat", &iat);
    printf("Hello $.*S!", name.len, name.p);
  }
