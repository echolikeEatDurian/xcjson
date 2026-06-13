//
// Created by echo on 2026/6/13.
//
#include "test_cjson_scanner.h"
#include "../Cjson.h"
#include "unity/unity.h"


/* ================================================================
 * 1. json_scanner_init_scanner 测试
 * ================================================================ */

void test_json_scanner_init_sets_start_and_current_to_source(void) {
    struct JsonTokenScanner scanner;
    const char *src = "abc";
    json_scanner_init_scanner(&scanner, src);

    TEST_ASSERT_EQUAL_PTR(src, scanner.start);
    TEST_ASSERT_EQUAL_PTR(src, scanner.current);
}

void test_json_scanner_init_initialises_line_to_1(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    TEST_ASSERT_EQUAL_INT(1, scanner.row);
}

void test_json_scanner_init_initialises_col_to_1(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    TEST_ASSERT_EQUAL_INT(1, scanner.col);
}

/* ================================================================
 * 2. json_scanner_advance 测试
 * ================================================================ */

void test_json_scanner_advance_returns_current_char(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    char c = json_scanner_advance(&scanner);
    TEST_ASSERT_EQUAL_CHAR('a', c);
}

void test_json_scanner_advance_moves_current_pointer(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    json_scanner_advance(&scanner);
    TEST_ASSERT_EQUAL_CHAR('b', *scanner.current);
}

void test_json_scanner_advance_increments_col(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    json_scanner_advance(&scanner);
    TEST_ASSERT_EQUAL_INT(2, scanner.col);
}

/* ================================================================
 * 3. json_scanner_peek 测试
 * ================================================================ */

void test_json_scanner_peek_returns_current_char(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    TEST_ASSERT_EQUAL_CHAR('a', json_scanner_peek(&scanner));
}

void test_json_scanner_peek_does_not_advance_pointer(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    json_scanner_peek(&scanner);
    TEST_ASSERT_EQUAL_CHAR('a', *scanner.current);
}

void test_json_scanner_peek_returns_null_terminator_at_end(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "");
    TEST_ASSERT_EQUAL_CHAR('\0', json_scanner_peek(&scanner));
}

/* ================================================================
 * 4. json_scanner_skip_whitespace 测试
 * ================================================================ */

void test_json_scanner_skip_whitespace_skips_spaces(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "   hello");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('h', json_scanner_peek(&scanner));
}

void test_json_scanner_skip_whitespace_skips_tabs(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\t\tworld");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('w', json_scanner_peek(&scanner));
}

void test_json_scanner_skip_whitespace_skips_newlines(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\n\nfoo");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('f', json_scanner_peek(&scanner));
}

void test_json_scanner_skip_whitespace_skips_mixed_whitespace(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, " \t\r\n \tbar");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('b', json_scanner_peek(&scanner));
}

void test_json_scanner_skip_whitespace_increments_line_for_newline(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\nhello");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_INT(2, scanner.row);
}

void test_json_scanner_skip_whitespace_does_not_skip_regular_chars(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "hello");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('h', json_scanner_peek(&scanner));
}

void test_json_scanner_skip_whitespace_handles_only_whitespace(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "   \t");
    json_scanner_skip_whitespace(&scanner);
    TEST_ASSERT_EQUAL_CHAR('\0', json_scanner_peek(&scanner));
}

/* ================================================================
 * 5. json_scanner_next_json_token - 符号测试 (punctuation tokens)
 * ================================================================ */

void test_json_scanner_next_token_recognises_left_brace(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACE, token.token_type);
    TEST_ASSERT_EQUAL_STRING("{", token.value_string);
}

void test_json_scanner_next_token_recognises_right_brace(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "}");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_R_BRACE, token.token_type);
    TEST_ASSERT_EQUAL_STRING("}", token.value_string);
}

void test_json_scanner_next_token_recognises_left_bracket(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACKET, token.token_type);
    TEST_ASSERT_EQUAL_STRING("[", token.value_string);
}

void test_json_scanner_next_token_recognises_right_bracket(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "]");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_R_BRACKET, token.token_type);
    TEST_ASSERT_EQUAL_STRING("]", token.value_string);
}

void test_json_scanner_next_token_recognises_colon(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, ":");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_COLON, token.token_type);
    TEST_ASSERT_EQUAL_STRING(":", token.value_string);
}

void test_json_scanner_next_token_recognises_comma(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, ",");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_COMMA, token.token_type);
    TEST_ASSERT_EQUAL_STRING(",", token.value_string);
}

/* ================================================================
 * 6. json_scanner_next_json_token - 关键字 true/false/null 测试
 * ================================================================ */

void test_json_scanner_next_token_recognises_true_lowercase(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_TRUE, token.token_type);
}

void test_json_scanner_next_token_recognises_true_uppercase_T(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "True");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_TRUE, token.token_type);
}

void test_json_scanner_next_token_recognises_false_lowercase(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "false");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_FALSE, token.token_type);
}

void test_json_scanner_next_token_recognises_false_uppercase_F(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "False");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_FALSE, token.token_type);
}

void test_json_scanner_next_token_recognises_null_lowercase(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NULL, token.token_type);
}

void test_json_scanner_next_token_recognises_null_uppercase_N(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "Null");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NULL, token.token_type);
}

/* ================================================================
 * 7. json_scanner_next_json_token - 字符串 token 测试
 * ================================================================ */

void test_json_scanner_next_token_recognises_simple_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello\"");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, token.token_type);
    TEST_ASSERT_EQUAL_STRING("hello", token.value_string);
}

void test_json_scanner_next_token_recognises_empty_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"\"");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, token.token_type);
    TEST_ASSERT_EQUAL_STRING("", token.value_string);
}

void test_json_scanner_next_token_recognises_string_with_escape_sequence(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello\\nworld\"");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, token.token_type);
    TEST_ASSERT_EQUAL_STRING("hello\\nworld", token.value_string);
}

void test_json_scanner_next_token_recognises_string_with_spaces(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello world\"");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, token.token_type);
    TEST_ASSERT_EQUAL_STRING("hello world", token.value_string);
}

/* ================================================================
 * 8. json_scanner_next_json_token - 数字 token 测试
 * ================================================================ */

void test_json_scanner_next_token_recognises_integer_number(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "123");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(123.0, token.value_number);
    
}

void test_json_scanner_next_token_recognises_zero(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "0");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, token.value_number);
}

void test_json_scanner_next_token_recognises_decimal_number(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "3.14");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, token.value_number);
}

void test_json_scanner_next_token_recognises_negative_number(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "-42");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(-42.0, token.value_number);
}

void test_json_scanner_next_token_recognises_negative_decimal(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "-3.14");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(-3.14, token.value_number);
}

void test_json_scanner_next_token_recognises_scientific_notation_with_e(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "1e3");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(1000.0, token.value_number);
}

void test_json_scanner_next_token_recognises_scientific_notation_with_uppercase_E(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "1E3");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(1000.0, token.value_number);
}

void test_json_scanner_next_token_recognises_scientific_notation_with_plus(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "2e+2");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(200.0, token.value_number);
}

void test_json_scanner_next_token_recognises_scientific_notation_with_minus(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "2e-2");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, token.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(0.02, token.value_number);
}

void test_json_scanner_next_token_number_value_string_matches_input(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "3.14");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_STRING("3.14", token.value_string);
}

/* ================================================================
 * 9. json_scanner_next_json_token - EOF 测试
 * ================================================================ */

void test_json_scanner_next_token_returns_EOF_for_empty_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_EOF, token.token_type);
}

void test_json_scanner_next_token_returns_EOF_after_whitespace(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "   ");
    struct JsonToken token = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_EOF, token.token_type);
}

/* ================================================================
 * 10. json_scanner_try_parse_string - 错误场景测试（不通过 json_scanner_next_json_token）
 * ================================================================ */

void test_json_scanner_try_parse_string_returns_error_for_unterminated_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "\"hello");
    int success = -1;
    struct JsonToken token = json_scanner_try_parse_string(&scanner);
  
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_ERROR, token.token_type);
}

/* ================================================================
 * 11. json_scanner_try_parse_true - 错误场景测试
 * ================================================================ */

void test_json_scanner_try_parse_true_returns_error_for_invalid_prefix(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "trux");
    struct JsonToken token = json_scanner_try_parse_true(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_ERROR, token.token_type);
}

void test_json_scanner_try_parse_true_returns_success_for_valid(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "true");
    struct JsonToken token = json_scanner_try_parse_true(&scanner);

    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_TRUE, token.token_type);
}

/* ================================================================
 * 12. json_scanner_try_parse_false - 错误场景测试
 * ================================================================ */

void test_json_scanner_try_parse_false_returns_error_for_invalid_prefix(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "falsx");
    int success = -1;
    struct JsonToken token = json_scanner_try_parse_false(&scanner);
  
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_ERROR, token.token_type);
}

void test_json_scanner_try_parse_false_returns_success_for_valid(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "false");
    int success = -1;
    struct JsonToken token = json_scanner_try_parse_false(&scanner);

    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_FALSE, token.token_type);
}

/* ================================================================
 * 13. json_scanner_try_parse_null - 错误场景测试
 * ================================================================ */

void test_json_scanner_try_parse_null_returns_error_for_invalid_prefix(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "nulx");
    int success = -1;
    struct JsonToken token = json_scanner_try_parse_null(&scanner);
  
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_ERROR, token.token_type);
}

void test_json_scanner_try_parse_null_returns_success_for_valid(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "null");
    int success = -1;
    struct JsonToken token = json_scanner_try_parse_null(&scanner);

    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NULL, token.token_type);
}

/* ================================================================
 * 14. json_scanner_new_json_token - Token 创建测试
 * ================================================================ */

void test_json_token_new_sets_token_type(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{");
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_L_BRACE);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACE, token.token_type);
}

void test_json_token_new_sets_length_correctly(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "123");
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_NUMBER);
    TEST_ASSERT_EQUAL_INT(3, token.length);
}

void test_json_token_new_copies_value_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "42");
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_NUMBER);
    TEST_ASSERT_EQUAL_STRING("42", token.value_string);
}

void test_json_token_new_sets_line_and_col(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{");
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_L_BRACE);
    TEST_ASSERT_EQUAL_INT(1, token.row);
    TEST_ASSERT_EQUAL_INT(2, token.col);
}

void test_json_token_new_number_sets_value_double(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "42");
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_NUMBER);
    TEST_ASSERT_EQUAL_DOUBLE(42.0, token.value_number);
}

void test_json_token_new_null_terminates_value_string(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "abc");
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    json_scanner_advance(&scanner);
    struct JsonToken token = json_scanner_new_json_token(&scanner, JSON_TOKEN_STRING);
    TEST_ASSERT_EQUAL_CHAR('\0', token.value_string[token.length]);
}

/* ================================================================
 * 15. 集成测试 - 多 token 序列解析
 * ================================================================ */

void test_integration_tokenises_simple_object(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\"key\": 1}");

    struct JsonToken t1 = json_scanner_next_json_token(&scanner);
    struct JsonToken t2 = json_scanner_next_json_token(&scanner);
    struct JsonToken t3 = json_scanner_next_json_token(&scanner);
    struct JsonToken t4 = json_scanner_next_json_token(&scanner);
    struct JsonToken t5 = json_scanner_next_json_token(&scanner);

    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACE, t1.token_type);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, t2.token_type);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_COLON, t3.token_type);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, t4.token_type);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_R_BRACE, t5.token_type);
}

void test_integration_tokenises_simple_array(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "[1, 2, 3]");

    struct JsonToken t1 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACKET, t1.token_type);

    struct JsonToken t2 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, t2.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, t2.value_number);

    struct JsonToken t3 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_COMMA, t3.token_type);

    struct JsonToken t4 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_NUMBER, t4.token_type);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, t4.value_number);
}

void test_integration_tokenises_with_newlines_and_spaces(void) {
    struct JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner, "{\n  \"a\": true\n}");

    struct JsonToken t1 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_L_BRACE, t1.token_type);

    struct JsonToken t2 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_STRING, t2.token_type);

    struct JsonToken t3 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_COLON, t3.token_type);

    struct JsonToken t4 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_TRUE, t4.token_type);

    struct JsonToken t5 = json_scanner_next_json_token(&scanner);
    TEST_ASSERT_EQUAL_INT(JSON_TOKEN_R_BRACE, t5.token_type);
}

void test_integration_token_counts_for_complex_json(void) {
    struct JsonTokenScanner scanner;
    int token_count = 0;

    json_scanner_init_scanner(&scanner, "{\"name\": \"JSON Parser\", \"value\": 123, "
                          "\"active\": true, \"debug\": false, \"extra\": null, "
                          "\"list\": [1, 2, 3]}");

    while (1) {
        struct JsonToken token = json_scanner_next_json_token(&scanner);
        token_count++;
        if (token.token_type == JSON_TOKEN_EOF) break;
    }

    TEST_ASSERT_EQUAL_INT(32, token_count);
}
