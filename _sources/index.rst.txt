.. jsmn-web-tokens documentation master file, created by
   sphinx-quickstart on Sun Feb  9 07:35:25 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. rst-class:: align-center margin-bottom-20 
.. container::

  .. image:: _static/img/jwt.svg
     :scale: 150%
     :align: center

Jasmine Web Tokens
==================

.. code-block:: c

  // Create token
  jsmn_token_init(&token, JSMN_ALG_HS256, "{\"sub\":\"%s\",\"iat\":%d}", "user", time(NULL));

  // Decode and validate token
  jsmn_token_decode(&token, "your-256-bit-secret", JSMN_ALG_HS256, jwt, strlen(jwt));

.. image:: https://travis-ci.com/TomzBench/jsmn-web-tokens.svg?branch=master
    :target: https://travis-ci.com/TomzBench/jsmn-web-tokens
.. image:: https://codecov.io/gh/TomzBench/jsmn-web-tokens/branch/master/graph/badge.svg
    :target: https://codecov.io/gh/TomzBench/jsmn-web-tokens

About
-----

jsmn-web-tokens is a small library for encoding and decoding `Json Web Tokens <https://jwt.io>`_ intended for light weight embedded systems. jsmn-web-tokens relies on crypto primitives such as MBedTLS or WolfSSL which are targeted for constrained devices. jsmn-web-tokens can be configured to not rely on heap memory and has configuration options that are friendly toward IOT to control stack size.

Getting Started
---------------

.. code-block:: bash

   git clone https://github.com/TomzBench/jsmn-web-tokens.git
   cd jsmn-web-tokens
   git submodule update --init
   mkdir build
   cd build
   cmake ..
   make
   make install

Configuration Options
---------------------

* **BUILD_SHARED**

    Enable shared library output (IE: /lib/libjsmn-web-tokens.so) 
    
    *Default: ON*

* **BUILD_STATIC**

    Enable static library output (IE: /lib/libjsmn-web-tokens.a) 
    
    *Default: ON*

* **WITH_SYSTEM_DEPENDENCIES**

    jsmn-web-tokens has a crypto library dependency.  CMake can find this dependency located on your host system or install directory, - OR - can be configured to download and build this dependency locally (recommended for reproducible builds...). When WITH_SYSTEM_DEPENDENCIES is configured as ON, CMake will not compile the crypto library and therefore you must have this dependency installed already. (IE: sudo apt-get install libssl-dev or equivalent)  
    
    *Default: OFF*

* **JSMN_MAX_TOKEN_LEN**

    jsmn-web-tokens uses the stack to decode and encode tokens.  Therefore stack variables must be pre-allocated to support a maximum length. Since jsmn-web-tokens is intended for smaller systems, the default value is set pretty low. If you need to parse very large tokens you should use a different library ;). 
    
    *Default: 256*

* **JSMN_MAX_TOKENS**

    jsmn-web-tokens uses `jsmn <https://github.com/zserge/jsmn>`_ to parse JSON. (duh!) And jsmn also uses the stack to decode JSON objects. Therefore jsmn-web-tokens must preallocate memory to parse JSON. Symbols such as '{' and 'thing' are called tokens. So you should set this number to the largest amount of tokens you need to parse.

    *Default: 64*

    .. note:: Jsmn "Tokens" are not to be confused with JSON Web "Tokens" :P

* **WITH_CRYPTO**

    Select a crypto library backend to link against. Valid options are:

    * MBEDTLS

    * WOLFSSL

    * OPENSSL

    *Default: MBEDTLS*

* **ENABLE_TESTING**

    Build the test suite (Linux Only)

    *Default: OFF*

Examples
--------

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
  
   const char *jwt = \
   eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.\ 
   eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.\
   SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c;

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


.. toctree::
   :maxdepth: 1
   :caption: API

   api_jsmn_alg.rst
   api_jsmn_value.rst
   api_jsmn_token_encode_s.rst
   api_jsmn_token_decode_s.rst
   api_jsmn_token_init.rst
   api_jsmn_token_sign.rst
   api_jsmn_token_decode.rst
   api_jsmn_token_get_claim_int.rst
   api_jsmn_token_get_claim_str.rst
