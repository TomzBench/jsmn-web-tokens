#include "jsmn/jsmn_helpers.h"
#include "jsmn/jsmn_tokens.h"

#include <setjmp.h>

#include <cmocka.h>

#define UNSAFE_ISS "jsmn_web_token_iss"
#define UNSAFE_SUB "jsmn_web_token_sub"
#define UNSAFE_IAT 1580601600
#define UNSAFE_EXP 1580602200
#define UNSAFE_SECRET "unsafe_secret"

/*
 * This token was taken with the following data from https://jwt.io
 * {
 *      "alg": "HS256",
 *      "typ": "JWT",
 * }
 * .
 * {
 *      "iss": "jsmn_web_token_iss",
 *      "sub": "jsmn_web_token_sub",
 *      "iat": 1580601600,
 *      "exp": 1580602200
 * }
 * .
 * HMACSHA256(
 *      base64UrlEncode(header) + "." +
 *      base64UrlEncode(payload),
 *      unsafe_secret
 * )
 *
 * base64 encode secret (false)
 */

#define EXPECT_HEADER_HS256 "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"
#define EXPECT_PAYLOAD_HS256                                                   \
    "eyJpc3MiOiJqc21uX3dlYl90b2tlbl9pc3MiLCJzdWIiOiJqc21uX3dlYl90b2tlbl9zdWIi" \
    "LCJpYXQiOjE1ODA2MDE2MDAsImV4cCI6MTU4MDYwMjIwMH0"
#define EXPECT_SIGNATURE_HS256 "mrv4Vr2tdq7W-cQAcUqvsjmj7_GvAvgJ-RgNa7HfjeQ"
#define EXPECT_TOKEN_HS256                                                     \
    EXPECT_HEADER_HS256 "." EXPECT_PAYLOAD_HS256 "." EXPECT_SIGNATURE_HS256

#define EXPECT_HEADER_HS384 "eyJhbGciOiJIUzM4NCIsInR5cCI6IkpXVCJ9"
#define EXPECT_PAYLOAD_HS384                                                   \
    "eyJpc3MiOiJqc21uX3dlYl90b2tlbl9pc3MiLCJzdWIiOiJqc21uX3dlYl90b2tlbl9zdWIi" \
    "LCJpYXQiOjE1ODA2MDE2MDAsImV4cCI6MTU4MDYwMjIwMH0"
#define EXPECT_SIGNATURE_HS384                                                 \
    "AI-BffUT9ymD44cIvsankVLBu89weikome1o4UQTHYgwftBgSXj30uqHEU5pfGws"
#define EXPECT_TOKEN_HS384                                                     \
    EXPECT_HEADER_HS384 "." EXPECT_PAYLOAD_HS384 "." EXPECT_SIGNATURE_HS384

#define EXPECT_HEADER_HS512 "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9"
#define EXPECT_PAYLOAD_HS512                                                   \
    "eyJpc3MiOiJqc21uX3dlYl90b2tlbl9pc3MiLCJzdWIiOiJqc21uX3dlYl90b2tlbl9zdWIi" \
    "LCJpYXQiOjE1ODA2MDE2MDAsImV4cCI6MTU4MDYwMjIwMH0"
#define EXPECT_SIGNATURE_HS512                                                 \
    "QpBq98RW3RqzhG8b4HXeTHEAybMxMTbwuT0OvyWF9XOlzHlgHLfTJ08UWgTJy7j_"         \
    "I8W8okaB6iDWvb1wUcsreA"
#define EXPECT_TOKEN_HS512                                                     \
    EXPECT_HEADER_HS512 "." EXPECT_PAYLOAD_HS512 "." EXPECT_SIGNATURE_HS512

#define TOKEN_TOO_LONG                                                         \
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."                                    \
    "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwidGVzdCI6WzEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAs" \
    "MTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEwMCwxMDAsMTAwLDEw" \
    "MCwxMDAsMTAwLDEwMCwxMDAsMTAwXSwiaWF0IjoxNTE2MjM5MDIyfQ."                  \
    "QCaRWZgpXueiSgaSVEe9Gsu_A1LoDC-ZizaJjyIXM04"

#define PAYLOAD_FMT_STR                                                        \
    "{"                                                                        \
    "\"iss\":\"%s\","                                                          \
    "\"sub\":\"%s\","                                                          \
    "\"iat\":%d,"                                                              \
    "\"exp\":%d"                                                               \
    "}"

static void
test_jsmn_token_init_ok(void** context_p)
{
    ((void)context_p);

    int err;
    jsmn_token_encode_s t;
    JSMN_ALG algs[] = { JSMN_ALG_HS256, JSMN_ALG_HS384, JSMN_ALG_HS512 };
    const char* payloads[] = { EXPECT_HEADER_HS256 "." EXPECT_PAYLOAD_HS256,
                               EXPECT_HEADER_HS384 "." EXPECT_PAYLOAD_HS384,
                               EXPECT_HEADER_HS512 "." EXPECT_PAYLOAD_HS512 };
    const char* tokens[] = { EXPECT_TOKEN_HS256,
                             EXPECT_TOKEN_HS384,
                             EXPECT_TOKEN_HS512 };

    for (int i = 0; i < 3; i++) {

        err = jsmn_token_init(
            &t,
            algs[i],
            PAYLOAD_FMT_STR,
            UNSAFE_ISS,
            UNSAFE_SUB,
            UNSAFE_IAT,
            UNSAFE_EXP);

        // Verify header + payload
        assert_int_equal(err, 0);
        assert_int_equal(strlen(payloads[i]), t.len);
        assert_memory_equal(payloads[i], t.b, t.len);

        // Verify signature
        err = jsmn_token_sign(&t, UNSAFE_SECRET, strlen(UNSAFE_SECRET));
        assert_int_equal(err, 0);
        assert_int_equal(strlen(tokens[i]), t.len);
        assert_memory_equal(tokens[i], jsmn_token_data(&t), jsmn_token_len(&t));
    }
}

static void
test_jsmn_token_decode_ok(void** context_p)
{
    int err, iat, exp;
    jsmn_value iss, sub;
    jsmn_token_decode_s token;
    JSMN_ALG algs[] = { JSMN_ALG_HS256, JSMN_ALG_HS384, JSMN_ALG_HS512 };
    const char* tokens[] = { EXPECT_TOKEN_HS256,
                             EXPECT_TOKEN_HS384,
                             EXPECT_TOKEN_HS512 };

    for (int i = 0; i < 3; i++) {
        err = jsmn_token_decode(
            &token, UNSAFE_SECRET, algs[i], tokens[i], strlen(tokens[i]));

        assert_int_equal(err, 0);
        assert_int_equal(token.alg, algs[i]);
        assert_int_equal(jsmn_token_get_claim_str(&token, "iss", &iss), 0);
        assert_int_equal(jsmn_token_get_claim_str(&token, "sub", &sub), 0);
        assert_int_equal(jsmn_token_get_claim_int(&token, "iat", &iat), 0);
        assert_int_equal(jsmn_token_get_claim_int(&token, "exp", &exp), 0);
        assert_memory_equal(iss.p, UNSAFE_ISS, iss.len);
        assert_memory_equal(sub.p, UNSAFE_SUB, sub.len);
        assert_int_equal(iat, UNSAFE_IAT);
        assert_int_equal(exp, UNSAFE_EXP);
    }
}

static void
test_jsmn_token_decode_fail_sig(void** context_p)
{
    jsmn_token_decode_s token;
    int err;

    err = jsmn_token_decode(
        &token,
        UNSAFE_SECRET,               // secret is correct
        JSMN_ALG_HS256,              // using CORRECT algorithm
        EXPECT_TOKEN_HS256,          //
        strlen(EXPECT_TOKEN_HS256)); //
    assert_int_equal(err, 0);        // worky

    err = jsmn_token_decode(
        &token,
        UNSAFE_SECRET,               // secret is correct
        JSMN_ALG_HS512,              // using WRONG algorithm
        EXPECT_TOKEN_HS256,          //
        strlen(EXPECT_TOKEN_HS256)); //
    assert_int_equal(err, -1);       // no worky
}

static void
test_jsmn_token_decode_fail_too_long(void** context_p)
{
    jsmn_token_decode_s token;
    int err;

    err = jsmn_token_decode(
        &token,
        UNSAFE_SECRET,
        JSMN_ALG_HS256,
        TOKEN_TOO_LONG,
        strlen(TOKEN_TOO_LONG));
    assert_int_equal(err, -1);
}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_jsmn_token_init_ok),
        cmocka_unit_test(test_jsmn_token_decode_ok),
        cmocka_unit_test(test_jsmn_token_decode_fail_sig),
        cmocka_unit_test(test_jsmn_token_decode_fail_too_long),
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
