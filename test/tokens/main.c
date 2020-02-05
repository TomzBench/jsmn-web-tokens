#include "jsmn_helpers.h"
#include "jsmn_web_token/jsmn_web_token.h"

#include <setjmp.h>

#include <cmocka.h>

#define UNSAFE_ISS "jsmn_web_token_iss"
#define UNSAFE_SUB "jsmn_web_token_sub"
#define UNSAFE_IAT 1580601600
#define UNSAFE_EXP 1580602200

#define EXPECT_HEADER "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"
#define EXPECT_PAYLOAD                                                         \
    "eyJpc3MiOiJqc21uX3dlYl90b2tlbl9pc3MiLCJzdWIiOiJqc21uX3dlYl90b2tlbl9zdWIi" \
    "LCJpYXQiOjE1ODA2MDE2MDAsImV4cCI6MTU4MDYwMjIwMH0"
#define EXPECT_SIGNATURE "mrv4Vr2tdq7W-cQAcUqvsjmj7_GvAvgJ-RgNa7HfjeQ"
#define EXPECT_TOKEN EXPECT_HEADER "." EXPECT_PAYLOAD

#define PAYLOAD_FMT_STR                                                        \
    "{"                                                                        \
    "\"iss\":\"%s\","                                                          \
    "\"sub\":\"%s\","                                                          \
    "\"iat\":%d,"                                                              \
    "\"exp\":%d"                                                               \
    "}"

static void
test_jsmn_web_token_init_ok(void** context_p)
{
    ((void)context_p);

    int err;
    jsmn_web_token_s token;
    const char* expect = EXPECT_TOKEN;

    err = jsmn_web_token_init(
        &token,
        JSMN_WEB_TOKEN_ALG_HS256,
        PAYLOAD_FMT_STR,
        UNSAFE_ISS,
        UNSAFE_SUB,
        UNSAFE_IAT,
        UNSAFE_EXP);

    assert_int_equal(err, 0);
    assert_int_equal(strlen(EXPECT_TOKEN), token.len);
}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_jsmn_web_token_init_ok),
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
