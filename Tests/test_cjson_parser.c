//
// Created by echo on 2026/6/13.
//
#include "test_cjson_parser.h"
#include "../Cjson.h"
#include "unity/unity.h"
#include <string.h>
#include <stdlib.h>

/* ================================================================
 * 1. json_parser_init 测试
 * ================================================================ */

void test_json_parser_init_sets_scanner(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    TEST_ASSERT_EQUAL_PTR(&scanner, parser.scanner);
}

void test_json_parser_init_sets_has_error_to_false(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    TEST_ASSERT_EQUAL_INT(0, parser.has_error);
}

void test_json_parser_init_sets_error_msg_to_null(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    TEST_ASSERT_NULL(parser.error_msg);
}

/* ================================================================
 * 2. json_parser_parse - 完整解析测试
 * ================================================================ */

void test_json_parser_parse_parses_null(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NULL, node->type);
}

void test_json_parser_parse_parses_true(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_TRUE, node->type);
}

void test_json_parser_parse_parses_false(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "false");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_FALSE, node->type);
}

void test_json_parser_parse_parses_string(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello\"");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_STRING, node->type);
    TEST_ASSERT_EQUAL_STRING("hello", node->value_string);
}

void test_json_parser_parse_parses_number(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "42");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(42.0, node->value_number);
}

void test_json_parser_parse_parses_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"key\": \"value\"}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
    TEST_ASSERT_EQUAL_INT(1, node->Object.count);
}

void test_json_parser_parse_parses_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[1, 2, 3]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
    TEST_ASSERT_EQUAL_INT(3, node->Array.count);
}

/* ================================================================
 * 3. json_parser_parse_value 测试
 * ================================================================ */

void test_json_parser_parse_value_parses_null(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NULL, node->type);
}

void test_json_parser_parse_value_parses_true(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_TRUE, node->type);
}

void test_json_parser_parse_value_parses_false(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "false");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_FALSE, node->type);
}

void test_json_parser_parse_value_parses_string(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"test\"");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_STRING, node->type);
}

void test_json_parser_parse_value_parses_number(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "3.14");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, node->value_number);
}

void test_json_parser_parse_value_parses_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
}

void test_json_parser_parse_value_parses_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_value(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
}

/* ================================================================
 * 4. json_parser_parse_object 测试
 * ================================================================ */

void test_json_parser_parse_object_parses_empty_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_object(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
    TEST_ASSERT_EQUAL_INT(0, node->Object.count);
}

void test_json_parser_parse_object_parses_single_member(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"name\": \"value\"}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_object(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
    TEST_ASSERT_EQUAL_INT(1, node->Object.count);
    TEST_ASSERT_EQUAL_STRING("name", node->Object.keys[0]);
}

void test_json_parser_parse_object_parses_multiple_members(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"a\": 1, \"b\": 2}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_object(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
    TEST_ASSERT_EQUAL_INT(2, node->Object.count);
}

void test_json_parser_parse_object_parses_nested_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"outer\": {\"inner\": 1}}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_object(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->type);
    TEST_ASSERT_EQUAL_INT(1, node->Object.count);
    TEST_ASSERT_EQUAL_INT(JSON_OBJECT, node->Object.values[0]->type);
}

/* ================================================================
 * 5. json_parser_parse_array 测试
 * ================================================================ */

void test_json_parser_parse_array_parses_empty_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_array(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
    TEST_ASSERT_EQUAL_INT(0, node->Array.count);
}

void test_json_parser_parse_array_parses_single_element(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[42]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_array(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
    TEST_ASSERT_EQUAL_INT(1, node->Array.count);
    TEST_ASSERT_EQUAL_DOUBLE(42.0, node->Array.elements[0]->value_number);
}

void test_json_parser_parse_array_parses_multiple_elements(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[1, 2, 3]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_array(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
    TEST_ASSERT_EQUAL_INT(3, node->Array.count);
}

void test_json_parser_parse_array_parses_nested_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[[1, 2], [3, 4]]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_array(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->type);
    TEST_ASSERT_EQUAL_INT(2, node->Array.count);
    TEST_ASSERT_EQUAL_INT(JSON_ARRAY, node->Array.elements[0]->type);
}

/* ================================================================
 * 6. json_parser_parse_string 测试
 * ================================================================ */

void test_json_parser_parse_string_parses_simple_string(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello world\"");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_string(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_STRING, node->type);
    TEST_ASSERT_EQUAL_STRING("hello world", node->value_string);
}

void test_json_parser_parse_string_parses_empty_string(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"\"");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_string(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_STRING, node->type);
    TEST_ASSERT_EQUAL_STRING("", node->value_string);
}

void test_json_parser_parse_string_parses_string_with_escape(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello\\nworld\"");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_string(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_STRING, node->type);
}

/* ================================================================
 * 7. json_parser_parse_number 测试
 * ================================================================ */

void test_json_parser_parse_number_parses_integer(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "123");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_number(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(123.0, node->value_number);
}

void test_json_parser_parse_number_parses_decimal(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "3.14159");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_number(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, node->value_number);
}

void test_json_parser_parse_number_parses_negative(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "-100");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_number(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(-100.0, node->value_number);
}

void test_json_parser_parse_number_parses_zero(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "0");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_number(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NUMBER, node->type);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, node->value_number);
}

/* ================================================================
 * 8. json_parser_parse_true/false/null 测试
 * ================================================================ */

void test_json_parser_parse_true_returns_json_true(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_true(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_TRUE, node->type);
}

void test_json_parser_parse_false_returns_json_false(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "false");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_false(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_FALSE, node->type);
}

void test_json_parser_parse_null_returns_json_null(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse_null(&parser);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(JSON_NULL, node->type);
}

/* ================================================================
 * 9. json_parser_expect 测试
 * ================================================================ */

void test_json_parser_expect_returns_true_for_matching_token(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{");
    json_parser_init(&parser, &scanner);
    int result = json_parser_expect(&parser, JSON_TOKEN_L_BRACE);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_json_parser_expect_returns_false_for_mismatched_token(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{");
    json_parser_init(&parser, &scanner);
    parser.currentToken = json_scanner_next_json_token(&scanner);
    int result = json_parser_expect(&parser, JSON_TOKEN_L_BRACKET);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ================================================================
 * 10. json_writer_init 测试
 * ================================================================ */



void test_json_writer_init_sets_length_to_zero(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    TEST_ASSERT_EQUAL_INT(0, writer.length);
}



/* ================================================================
 * 11. json_writer_expand 测试
 * ================================================================ */

void test_json_writer_expand_increases_capacity(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_expand(&writer, 16);
    TEST_ASSERT(writer.capacity >= 16);
}

void test_json_writer_expand_preserves_existing_content(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "hello");
    size_t old_length = writer.length;
    json_writer_expand(&writer, 100);
    TEST_ASSERT_EQUAL_INT((int)old_length, (int)writer.length);
    TEST_ASSERT_EQUAL_STRING("hello", writer.buffer);
}

/* ================================================================
 * 12. json_writer_append_char 测试
 * ================================================================ */

void test_json_writer_append_char_appends_single_char(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_char(&writer, 'a');
    TEST_ASSERT_EQUAL_CHAR('a', writer.buffer[0]);
}

void test_json_writer_append_char_updates_length(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_char(&writer, 'a');
    json_writer_append_char(&writer, 'b');
    TEST_ASSERT_EQUAL_INT(2, (int)writer.length);
}

void test_json_writer_append_char_null_terminates(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_char(&writer, 'x');
    TEST_ASSERT_EQUAL_CHAR('\0', writer.buffer[1]);
}

/* ================================================================
 * 13. json_writer_append_string 测试
 * ================================================================ */

void test_json_writer_append_string_appends_string(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "hello");
    TEST_ASSERT_EQUAL_STRING("hello", writer.buffer);
}

void test_json_writer_append_string_updates_length(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "hello");
    TEST_ASSERT_EQUAL_INT(5, (int)writer.length);
}

void test_json_writer_append_string_handles_empty_string(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "");
    TEST_ASSERT_EQUAL_INT(0, writer.length);
}

/* ================================================================
 * 14. json_writer_release 测试
 * ================================================================ */

void test_json_writer_release_returns_buffer(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "test");
    char *result = json_writer_release(&writer);
    TEST_ASSERT_EQUAL_STRING("test", result);
    free(result);
}

void test_json_writer_release_null_terminates(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "abc");
    char *result = json_writer_release(&writer);
    TEST_ASSERT_EQUAL_CHAR('\0', result[3]);
    free(result);
}

void test_json_writer_release_resets_writer_state(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_writer_append_string(&writer, "data");
    char* buffer = json_writer_release(&writer);
    writer.length = 0;
    writer.capacity= 0;
    free(buffer);
    writer.buffer= NULL;
    TEST_ASSERT_NULL(writer.buffer);
    TEST_ASSERT_EQUAL_INT(0, writer.length);
    TEST_ASSERT_EQUAL_INT(0, (int)writer.capacity);
}

/* ================================================================
 * 15. json_serialize_string 测试
 * ================================================================ */

void test_json_serialize_string_wraps_in_quotes(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_string(&writer, "hello");
    TEST_ASSERT_EQUAL_STRING("\"hello\"", writer.buffer);
}

void test_json_serialize_string_escapes_special_chars(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_string(&writer, "line\nbreak");
    TEST_ASSERT(writer.buffer != NULL);
}

void test_json_serialize_string_handles_empty_string(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_string(&writer, "");
    TEST_ASSERT_EQUAL_STRING("\"\"", writer.buffer);
}

/* ================================================================
 * 16. json_serialize_number 测试
 * ================================================================ */

void test_json_serialize_number_serializes_integer(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_number(&writer, 42.0);
    TEST_ASSERT_EQUAL_STRING("42", writer.buffer);
}

void test_json_serialize_number_serializes_decimal(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_number(&writer, 3.14);
    TEST_ASSERT(writer.buffer != NULL);
}

void test_json_serialize_number_serializes_negative(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_number(&writer, -10.0);
    TEST_ASSERT_EQUAL_STRING("-10", writer.buffer);
}

void test_json_serialize_number_serializes_zero(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_number(&writer, 0.0);
    TEST_ASSERT_EQUAL_STRING("0", writer.buffer);
}

/* ================================================================
 * 17. json_serialize_value 测试
 * ================================================================ */

void test_json_serialize_value_serializes_string(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_STRING;
    node.value_string = "test";
    json_serialize_value(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("\"test\"", writer.buffer);
}

void test_json_serialize_value_serializes_number(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_NUMBER;
    node.value_number = 99.0;
    json_serialize_value(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("99", writer.buffer);
}

void test_json_serialize_value_serializes_true(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_TRUE;
    json_serialize_value(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("true", writer.buffer);
}

void test_json_serialize_value_serializes_false(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_FALSE;
    json_serialize_value(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("false", writer.buffer);
}

void test_json_serialize_value_serializes_null(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_NULL;
    json_serialize_value(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("null", writer.buffer);
}

/* ================================================================
 * 18. json_serialize_object 测试
 * ================================================================ */

void test_json_serialize_object_serializes_empty_object(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_OBJECT;
    node.Object.keys = NULL;
    node.Object.values = NULL;
    node.Object.count = 0;
    json_serialize_object(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("{}", writer.buffer);
}

void test_json_serialize_object_serializes_single_member(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_OBJECT;
    node.Object.count = 1;
    node.Object.keys = malloc(sizeof(char *));
    node.Object.values = malloc(sizeof(struct Json *));
    node.Object.keys[0] = "key";
    node.Object.values[0] = malloc(sizeof(struct Json));
    node.Object.values[0]->type = JSON_STRING;
    node.Object.values[0]->value_string = "value";
    json_serialize_object(&node, &writer);
    TEST_ASSERT(writer.buffer != NULL);
    free(node.Object.keys);
    free(node.Object.values[0]);
    free(node.Object.values);
}

void test_json_serialize_object_serializes_multiple_members(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_OBJECT;
    node.Object.count = 2;
    node.Object.keys = malloc(sizeof(char *) * 2);
    node.Object.values = malloc(sizeof(struct Json *) * 2);
    node.Object.keys[0] = "a";
    node.Object.values[0] = malloc(sizeof(struct Json));
    node.Object.values[0]->type = JSON_NUMBER;
    node.Object.values[0]->value_number = 1.0;
    node.Object.keys[1] = "b";
    node.Object.values[1] = malloc(sizeof(struct Json));
    node.Object.values[1]->type = JSON_NUMBER;
    node.Object.values[1]->value_number = 2.0;
    json_serialize_object(&node, &writer);
    TEST_ASSERT(writer.buffer != NULL);
    free(node.Object.keys);
    free(node.Object.values[0]);
    free(node.Object.values[1]);
    free(node.Object.values);
}

/* ================================================================
 * 19. json_serialize_array 测试
 * ================================================================ */

void test_json_serialize_array_serializes_empty_array(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_ARRAY;
    node.Array.elements = NULL;
    node.Array.count = 0;
    json_serialize_array(&node, &writer);
    TEST_ASSERT_EQUAL_STRING("[]", writer.buffer);
}

void test_json_serialize_array_serializes_single_element(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_ARRAY;
    node.Array.count = 1;
    node.Array.elements = malloc(sizeof(struct Json *));
    node.Array.elements[0] = malloc(sizeof(struct Json));
    node.Array.elements[0]->type = JSON_NUMBER;
    node.Array.elements[0]->value_number = 42.0;
    json_serialize_array(&node, &writer);
    TEST_ASSERT(writer.buffer != NULL);
    free(node.Array.elements[0]);
    free(node.Array.elements);
}

void test_json_serialize_array_serializes_multiple_elements(void) {
    JsonWriter writer;
    json_writer_init(&writer);
    struct Json node;
    node.type = JSON_ARRAY;
    node.Array.count = 2;
    node.Array.elements = malloc(sizeof(struct Json *) * 2);
    node.Array.elements[0] = malloc(sizeof(struct Json));
    node.Array.elements[0]->type = JSON_NUMBER;
    node.Array.elements[0]->value_number = 1.0;
    node.Array.elements[1] = malloc(sizeof(struct Json));
    node.Array.elements[1]->type = JSON_NUMBER;
    node.Array.elements[1]->value_number = 2.0;
    json_serialize_array(&node, &writer);
    TEST_ASSERT(writer.buffer != NULL);
    free(node.Array.elements[0]);
    free(node.Array.elements[1]);
    free(node.Array.elements);
}

/* ================================================================
 * 20. json_serialize - 完整序列化测试
 * ================================================================ */

void test_json_serialize_serializes_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"name\": \"test\"}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT(result != NULL);
    TEST_ASSERT(result[0] == '{');
    free(result);
}

void test_json_serialize_serializes_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[1, 2, 3]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT(result != NULL);
    TEST_ASSERT(result[0] == '[');
    free(result);
}

void test_json_serialize_serializes_primitive(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT_EQUAL_STRING("true", result);
    free(result);
}

/* ================================================================
 * 21. 集成测试 - Parse/Serialize 往返测试
 * ================================================================ */

void test_integration_parse_serialize_object(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"key\": \"value\"}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT(result != NULL);
    TEST_ASSERT(strstr(result, "\"key\"") != NULL);
    free(result);
}

void test_integration_parse_serialize_array(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[1, 2, 3]");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT(result != NULL);
    free(result);
}

void test_integration_parse_serialize_nested(void) {
    JsonParser parser;
    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"outer\": {\"inner\": 1}}");
    json_parser_init(&parser, &scanner);
    struct Json *node = json_parser_parse(&parser);
    char *result = json_serialize(node);
    TEST_ASSERT(result != NULL);
    free(result);
}

void test_integration_parse_serialize_primitives(void) {
    const char *test_cases[] = {"null", "true", "false", "\"string\"", "42", "3.14"};
    for (int i = 0; i < 6; i++) {
        JsonParser parser;
        JsonTokenScanner scanner;
        json_scanner_init_scanner(&scanner, test_cases[i]);
        json_parser_init(&parser, &scanner);
        struct Json *node = json_parser_parse(&parser);
        char *result = json_serialize(node);
        TEST_ASSERT(result != NULL);
        free(result);
    }
}
