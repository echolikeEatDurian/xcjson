//
// Created by echo on 2026/6/13.
//

#ifndef CJSON_FORK_TEST_CJSON_SCANNER_H
#define CJSON_FORK_TEST_CJSON_SCANNER_H


/* 1. json_scanner_init */
void test_json_scanner_init_sets_start_and_current_to_source(void);
void test_json_scanner_init_initialises_line_to_1(void);
void test_json_scanner_init_initialises_col_to_1(void);

/* 2. json_scanner_advance */
void test_json_scanner_advance_returns_current_char(void);
void test_json_scanner_advance_moves_current_pointer(void);
void test_json_scanner_advance_increments_col(void);

/* 3. json_scanner_peek */
void test_json_scanner_peek_returns_current_char(void);
void test_json_scanner_peek_does_not_advance_pointer(void);
void test_json_scanner_peek_returns_null_terminator_at_end(void);

/* 4. json_scanner_skip_whitespace */
void test_json_scanner_skip_whitespace_skips_spaces(void);
void test_json_scanner_skip_whitespace_skips_tabs(void);
void test_json_scanner_skip_whitespace_skips_newlines(void);
void test_json_scanner_skip_whitespace_skips_mixed_whitespace(void);
void test_json_scanner_skip_whitespace_increments_line_for_newline(void);
void test_json_scanner_skip_whitespace_does_not_skip_regular_chars(void);
void test_json_scanner_skip_whitespace_handles_only_whitespace(void);

/* 5. json_scanner_next_token - 符号 */
void test_json_scanner_next_token_recognises_left_brace(void);
void test_json_scanner_next_token_recognises_right_brace(void);
void test_json_scanner_next_token_recognises_left_bracket(void);
void test_json_scanner_next_token_recognises_right_bracket(void);
void test_json_scanner_next_token_recognises_colon(void);
void test_json_scanner_next_token_recognises_comma(void);

/* 6. json_scanner_next_token - 关键字 */
void test_json_scanner_next_token_recognises_true_lowercase(void);
void test_json_scanner_next_token_recognises_true_uppercase_T(void);
void test_json_scanner_next_token_recognises_false_lowercase(void);
void test_json_scanner_next_token_recognises_false_uppercase_F(void);
void test_json_scanner_next_token_recognises_null_lowercase(void);
void test_json_scanner_next_token_recognises_null_uppercase_N(void);

/* 7. json_scanner_next_token - 字符串 */
void test_json_scanner_next_token_recognises_simple_string(void);
void test_json_scanner_next_token_recognises_empty_string(void);
void test_json_scanner_next_token_recognises_string_with_escape_sequence(void);
void test_json_scanner_next_token_recognises_string_with_spaces(void);

/* 8. json_scanner_next_token - 数字 */
void test_json_scanner_next_token_recognises_integer_number(void);
void test_json_scanner_next_token_recognises_zero(void);
void test_json_scanner_next_token_recognises_decimal_number(void);
void test_json_scanner_next_token_recognises_negative_number(void);
void test_json_scanner_next_token_recognises_negative_decimal(void);
void test_json_scanner_next_token_recognises_scientific_notation_with_e(void);
void test_json_scanner_next_token_recognises_scientific_notation_with_uppercase_E(void);
void test_json_scanner_next_token_recognises_scientific_notation_with_plus(void);
void test_json_scanner_next_token_recognises_scientific_notation_with_minus(void);
void test_json_scanner_next_token_number_value_string_matches_input(void);

/* 9. json_scanner_next_token - EOF */
void test_json_scanner_next_token_returns_EOF_for_empty_string(void);
void test_json_scanner_next_token_returns_EOF_after_whitespace(void);

/* 10. json_scanner_try_parse_string - 错误场景 */
void test_json_scanner_try_parse_string_returns_error_for_unterminated_string(void);

/* 11. json_scanner_try_parse_true */
void test_json_scanner_try_parse_true_returns_error_for_invalid_prefix(void);
void test_json_scanner_try_parse_true_returns_success_for_valid(void);

/* 12. json_scanner_try_parse_false */
void test_json_scanner_try_parse_false_returns_error_for_invalid_prefix(void);
void test_json_scanner_try_parse_false_returns_success_for_valid(void);

/* 13. json_scanner_try_parse_null */
void test_json_scanner_try_parse_null_returns_error_for_invalid_prefix(void);
void test_json_scanner_try_parse_null_returns_success_for_valid(void);

/* 14. json_token_new */
void test_json_token_new_sets_token_type(void);
void test_json_token_new_sets_length_correctly(void);
void test_json_token_new_copies_value_string(void);
void test_json_token_new_sets_line_and_col(void);
void test_json_token_new_number_sets_value_double(void);
void test_json_token_new_null_terminates_value_string(void);

/* 15. 集成测试 */
void test_integration_tokenises_simple_object(void);
void test_integration_tokenises_simple_array(void);
void test_integration_tokenises_with_newlines_and_spaces(void);
void test_integration_token_counts_for_complex_json(void);


#endif //CJSON_FORK_TEST_CJSON_SCANNER_H
