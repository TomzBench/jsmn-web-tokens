#include "jsmn_helpers.h"

#include <setjmp.h>

#include <cmocka.h>

static const char* data_obj = //
    "{"
    "\"hello\":\"value-hello\","
    "\"this\" :\"value-this\","
    "\"is\"   :\"value-is\","
    "\"a\"    :\"value-a\","
    "\"test\" :\"value-test\""
    "}";

static const char* data_path = //
    "{"
    "\"thisa\":{"
    "           \"thata\":{"
    "                     \"valA\":\"thisa-thata-vala\","
    "                     \"valB\":\"thisa-thata-valb\","
    "                     \"valC\":\"thisa-thata-valc\","
    "                     \"valD\":\"thisa-thata-vald\","
    "                     \"fooD\":{\"valD\":\"gotcha\"},"
    "                     \"valE\":\"thisa-thata-vale\""
    "           },"
    "           \"thatb\":{"
    "                     \"valA\":\"thisa-thatb-vala\","
    "                     \"valB\":\"thisa-thatb-valb\","
    "                     \"valC\":\"thisa-thatb-valc\","
    "                     \"valD\":\"thisa-thatb-vald\","
    "                     \"valE\":\"thisa-thatb-vale\""
    "           }"
    "},"
    "\"thatb\":{"
    "           \"thisa\":{"
    "                     \"valA\":\"thatb-thisa-vala\","
    "                     \"valB\":\"thatb-thisa-valb\","
    "                     \"valC\":\"thatb-thisa-valc\","
    "                     \"valD\":\"thatb-thisa-vald\","
    "                     \"valE\":\"thatb-thisa-vale\""
    "           },"
    "           \"thisb\":{"
    "                     \"valA\":\"thatb-thisb-vala\","
    "                     \"valB\":\"thatb-thisb-valb\","
    "                     \"valC\":\"thatb-thisb-valc\","
    "                     \"valD\":\"thatb-thisb-vald\","
    "                     \"valE\":\"thatb-thisb-vale\""
    "           }"
    "}"
    "}";

static const char* about_data =
    "{\"about\":{\"siteId\":\"Site "
    "ID\",\"prjVersion\":\"2.01.32\",\"productKey\":\"\",\"product\":\"LINQ2\","
    "\"mqxVersion\":\"4.2.0\",\"atxVersion\":\"2.02.01\",\"sslVersion\":\"3.13."
    "0\",\"webVersion\":\"2.00.00\",\"mfg\":\"Altronix\",\"user\":\"\",\"mac\":"
    "\"CC:CE:68:96:5D:EE\",\"sid\":\"cqAIZtK_PLxXdJZIkkMuBS-dfkobQkfeuEFl-"
    "3JyS94=\",\"iicAddr\":0,\"valid\":{\"mac\":0,\"web\":0,\"mode\":0,"
    "\"ready\":0,\"webhash\":\"\"},\"policies\":0,\"users\":{},\"address\":25}"
    "}";

static void
test_parse_obj(void** context_p)
{
    ((void)context_p);

    int count = 0;
    jsmn_value values[5];
    jsmntok_t t[30];

    // clang-format off
    count = jsmn_parse_tokens(
        t,
        30,
        data_obj,
        strlen(data_obj),
        5,
        "hello", &values[0],
        "this",  &values[1],
        "is",    &values[2],
        "a",     &values[3],
        "test",  &values[4]);
    // clang-format on

    assert_int_equal(count, 5);
    assert_int_equal(values[0].len, 11);
    assert_memory_equal(values[0].p, "value-hello", 11);
    assert_int_equal(values[1].len, 10);
    assert_memory_equal(values[1].p, "value-this", 10);
    assert_int_equal(values[2].len, 8);
    assert_memory_equal(values[2].p, "value-is", 8);
    assert_int_equal(values[3].len, 7);
    assert_memory_equal(values[3].p, "value-a", 7);
    assert_int_equal(values[4].len, 10);
    assert_memory_equal(values[4].p, "value-test", 10);
}

static void
test_parse_path(void** context_p)
{
    ((void)context_p);

    int count = 0;
    jsmn_value values[5];
    jsmntok_t t[150];

    // clang-format off
    count = jsmn_parse_tokens_path(
        "/thisa/thata",
        t,
        150,
        data_path,
        strlen(data_path),
        5,
        "valA", &values[0],
        "valB", &values[1],
        "valC", &values[2],
        "valD", &values[3],
        "valE", &values[4]);
    // clang-format on

    assert_int_equal(count, 5);
    assert_int_equal(values[0].len, 16);
    assert_memory_equal(values[0].p, "thisa-thata-vala", 16);
    assert_int_equal(values[1].len, 16);
    assert_memory_equal(values[1].p, "thisa-thata-valb", 16);
    assert_int_equal(values[2].len, 16);
    assert_memory_equal(values[2].p, "thisa-thata-valc", 16);
    assert_int_equal(values[3].len, 16);
    assert_memory_equal(values[3].p, "thisa-thata-vald", 16);
    assert_int_equal(values[4].len, 16);
    assert_memory_equal(values[4].p, "thisa-thata-vale", 16);

    // clang-format off
    count = jsmn_parse_tokens_path(
        "/thisa/thatb",
        t,
        150,
        data_path,
        strlen(data_path),
        5,
        "valA", &values[0],
        "valB", &values[1],
        "valC", &values[2],
        "valD", &values[3],
        "valE", &values[4]);
    // clang-format on

    assert_int_equal(count, 5);
    assert_int_equal(values[0].len, 16);
    assert_memory_equal(values[0].p, "thisa-thatb-vala", 16);
    assert_int_equal(values[1].len, 16);
    assert_memory_equal(values[1].p, "thisa-thatb-valb", 16);
    assert_int_equal(values[2].len, 16);
    assert_memory_equal(values[2].p, "thisa-thatb-valc", 16);
    assert_int_equal(values[3].len, 16);
    assert_memory_equal(values[3].p, "thisa-thatb-vald", 16);
    assert_int_equal(values[4].len, 16);
    assert_memory_equal(values[4].p, "thisa-thatb-vale", 16);

    // clang-format off
    count = jsmn_parse_tokens_path(
        "/thatb/thisa",
        t,
        150,
        data_path,
        strlen(data_path),
        5,
        "valA", &values[0],
        "valB", &values[1],
        "valC", &values[2],
        "valD", &values[3],
        "valE", &values[4]);
    // clang-format on

    assert_int_equal(count, 5);
    assert_int_equal(values[0].len, 16);
    assert_memory_equal(values[0].p, "thatb-thisa-vala", 16);
    assert_int_equal(values[1].len, 16);
    assert_memory_equal(values[1].p, "thatb-thisa-valb", 16);
    assert_int_equal(values[2].len, 16);
    assert_memory_equal(values[2].p, "thatb-thisa-valc", 16);
    assert_int_equal(values[3].len, 16);
    assert_memory_equal(values[3].p, "thatb-thisa-vald", 16);
    assert_int_equal(values[4].len, 16);
    assert_memory_equal(values[4].p, "thatb-thisa-vale", 16);

    // clang-format off
    count = jsmn_parse_tokens_path(
        "/thatb/thisb",
        t,
        150,
        data_path,
        strlen(data_path),
        5,
        "valA", &values[0],
        "valB", &values[1],
        "valC", &values[2],
        "valD", &values[3],
        "valE", &values[4]);
    // clang-format on

    assert_int_equal(count, 5);
    assert_int_equal(values[0].len, 16);
    assert_memory_equal(values[0].p, "thatb-thisb-vala", 16);
    assert_int_equal(values[1].len, 16);
    assert_memory_equal(values[1].p, "thatb-thisb-valb", 16);
    assert_int_equal(values[2].len, 16);
    assert_memory_equal(values[2].p, "thatb-thisb-valc", 16);
    assert_int_equal(values[3].len, 16);
    assert_memory_equal(values[3].p, "thatb-thisb-vald", 16);
    assert_int_equal(values[4].len, 16);
    assert_memory_equal(values[4].p, "thatb-thisb-vale", 16);

    // clang-format off
    count = jsmn_parse_tokens_path(
        "thisa/notfound",
        t,
        150,
        data_path,
        strlen(data_path),
        5,
        "valA", &values[0],
        "valB", &values[1],
        "valC", &values[2],
        "valD", &values[3],
        "valE", &values[4]);
    // clang-format on
    assert_int_equal(count, 0);
}

static void
test_parse_about(void** context_p)
{
    ((void)context_p);

    int count = 0;
    jsmn_value values[5];
    jsmntok_t t[250];

    // clang-format off
    count = jsmn_parse_tokens_path(
        "/about",
        t,
        250,
        about_data,
        strlen(about_data),
        6,
        "sid",        &values[0],
        "product",    &values[1],
        "prjVersion", &values[2],
        "atxVersion", &values[3],
        "webVersion", &values[4],
        "mac",        &values[5]);
    // clang-format on

    assert_int_equal(count, 6);
}

static void
test_foreach_callback(void* pass, jsmn_value* key, jsmn_value* value)
{
    static int count = 1;
    if (count == 1) {
        assert_int_equal(key->len, 3);
        assert_memory_equal(key->p, "one", 3);
        count++;
    } else if (count == 2) {
        assert_int_equal(key->len, 3);
        assert_memory_equal(key->p, "two", 3);
        count++;
    } else {
        assert_int_equal(key->len, 5);
        assert_memory_equal(key->p, "three", 5);
        count = 0;
        *(bool*)pass = true;
    }
}

static void
test_foreach(void** context_p)
{
    ((void)context_p);
    bool pass = false;
    uint32_t n;
    const char* data = "{\"one\":1,\"haha\":{},\"two\":2,\"three\":3}";
    jsmn_parser p;
    jsmn_init(&p);
    jsmntok_t tokens[20];
    n = jsmn_parse(&p, data, strlen(data), tokens, 20);
    jsmn_foreach(tokens, n, data, test_foreach_callback, &pass);
    assert_true(pass);
}

int
main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);
    int err;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_obj),
        cmocka_unit_test(test_parse_path),
        cmocka_unit_test(test_parse_about),
        cmocka_unit_test(test_foreach),
    };

    err = cmocka_run_group_tests(tests, NULL, NULL);
    return err;
}
