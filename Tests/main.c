//
// Created by echo on 2026/6/13.
//

#include <stdio.h>

#include "unity/unity_internals.h"
#include "test_cjson_scanner.h"

void setUp(void) {


}

void tearDown(void) {


}

int main(int argc, char *argv[]) {
     (void)argc;
    (void)argv;

    printf("=============== run unit test ===============\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_json_scanner_init_sets_start_and_current_to_source);
    RUN_TEST(test_json_scanner_init_initialises_line_to_1);
    RUN_TEST(test_json_scanner_init_initialises_col_to_1);

    RUN_TEST(test_json_scanner_advance_returns_current_char);
    RUN_TEST(test_json_scanner_advance_moves_current_pointer);
    RUN_TEST(test_json_scanner_advance_increments_col);

    RUN_TEST(test_json_scanner_peek_returns_current_char);
    RUN_TEST(test_json_scanner_peek_does_not_advance_pointer);
    RUN_TEST(test_json_scanner_peek_returns_null_terminator_at_end);

    RUN_TEST(test_json_scanner_skip_whitespace_skips_spaces);
    RUN_TEST(test_json_scanner_skip_whitespace_skips_tabs);
    RUN_TEST(test_json_scanner_skip_whitespace_skips_newlines);
    RUN_TEST(test_json_scanner_skip_whitespace_skips_mixed_whitespace);
    RUN_TEST(test_json_scanner_skip_whitespace_increments_line_for_newline);
    RUN_TEST(test_json_scanner_skip_whitespace_does_not_skip_regular_chars);
    RUN_TEST(test_json_scanner_skip_whitespace_handles_only_whitespace);

    RUN_TEST(test_json_scanner_next_token_recognises_left_brace);
    RUN_TEST(test_json_scanner_next_token_recognises_right_brace);
    RUN_TEST(test_json_scanner_next_token_recognises_left_bracket);
    RUN_TEST(test_json_scanner_next_token_recognises_right_bracket);
    RUN_TEST(test_json_scanner_next_token_recognises_colon);
    RUN_TEST(test_json_scanner_next_token_recognises_comma);

    RUN_TEST(test_json_scanner_next_token_recognises_true_lowercase);
    RUN_TEST(test_json_scanner_next_token_recognises_true_uppercase_T);
    RUN_TEST(test_json_scanner_next_token_recognises_false_lowercase);
    RUN_TEST(test_json_scanner_next_token_recognises_false_uppercase_F);
    RUN_TEST(test_json_scanner_next_token_recognises_null_lowercase);
    RUN_TEST(test_json_scanner_next_token_recognises_null_uppercase_N);

    RUN_TEST(test_json_scanner_next_token_recognises_simple_string);
    RUN_TEST(test_json_scanner_next_token_recognises_empty_string);
    RUN_TEST(test_json_scanner_next_token_recognises_string_with_escape_sequence);
    RUN_TEST(test_json_scanner_next_token_recognises_string_with_spaces);

    RUN_TEST(test_json_scanner_next_token_recognises_integer_number);
    RUN_TEST(test_json_scanner_next_token_recognises_zero);
    RUN_TEST(test_json_scanner_next_token_recognises_decimal_number);
    RUN_TEST(test_json_scanner_next_token_recognises_negative_number);
    RUN_TEST(test_json_scanner_next_token_recognises_negative_decimal);
    RUN_TEST(test_json_scanner_next_token_recognises_scientific_notation_with_e);
    RUN_TEST(test_json_scanner_next_token_recognises_scientific_notation_with_uppercase_E);
    RUN_TEST(test_json_scanner_next_token_recognises_scientific_notation_with_plus);
    RUN_TEST(test_json_scanner_next_token_recognises_scientific_notation_with_minus);
    RUN_TEST(test_json_scanner_next_token_number_value_string_matches_input);

    RUN_TEST(test_json_scanner_next_token_returns_EOF_for_empty_string);
    RUN_TEST(test_json_scanner_next_token_returns_EOF_after_whitespace);

    RUN_TEST(test_json_scanner_try_parse_string_returns_error_for_unterminated_string);

    RUN_TEST(test_json_scanner_try_parse_true_returns_error_for_invalid_prefix);
    RUN_TEST(test_json_scanner_try_parse_true_returns_success_for_valid);

    RUN_TEST(test_json_scanner_try_parse_false_returns_error_for_invalid_prefix);
    RUN_TEST(test_json_scanner_try_parse_false_returns_success_for_valid);

    RUN_TEST(test_json_scanner_try_parse_null_returns_error_for_invalid_prefix);
    RUN_TEST(test_json_scanner_try_parse_null_returns_success_for_valid);

    RUN_TEST(test_json_token_new_sets_token_type);
    RUN_TEST(test_json_token_new_sets_length_correctly);
    RUN_TEST(test_json_token_new_copies_value_string);
    RUN_TEST(test_json_token_new_sets_line_and_col);
    RUN_TEST(test_json_token_new_number_sets_value_double);
    RUN_TEST(test_json_token_new_null_terminates_value_string);

    RUN_TEST(test_integration_tokenises_simple_object);
    RUN_TEST(test_integration_tokenises_simple_array);
    RUN_TEST(test_integration_tokenises_with_newlines_and_spaces);
    RUN_TEST(test_integration_token_counts_for_complex_json);

    return UNITY_END();
}
