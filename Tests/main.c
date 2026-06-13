//
// Created by echo on 2026/6/13.
//

#include <stdio.h>

#include "unity/unity_internals.h"
#include "test_cjson_scanner.h"
#include "test_cjson_parser.h"

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

    /* parser init tests */
    RUN_TEST(test_json_parser_init_sets_scanner);
    RUN_TEST(test_json_parser_init_sets_has_error_to_false);
    RUN_TEST(test_json_parser_init_sets_error_msg_to_null);

    /* json_parser_parse tests */
    RUN_TEST(test_json_parser_parse_parses_null);
    RUN_TEST(test_json_parser_parse_parses_true);
    RUN_TEST(test_json_parser_parse_parses_false);
    RUN_TEST(test_json_parser_parse_parses_string);
    RUN_TEST(test_json_parser_parse_parses_number);
    RUN_TEST(test_json_parser_parse_parses_object);
    RUN_TEST(test_json_parser_parse_parses_array);

    /* json_parser_parse_value tests */
    RUN_TEST(test_json_parser_parse_value_parses_null);
    RUN_TEST(test_json_parser_parse_value_parses_true);
    RUN_TEST(test_json_parser_parse_value_parses_false);
    RUN_TEST(test_json_parser_parse_value_parses_string);
    RUN_TEST(test_json_parser_parse_value_parses_number);
    RUN_TEST(test_json_parser_parse_value_parses_object);
    RUN_TEST(test_json_parser_parse_value_parses_array);

    /* json_parser_parse_object tests */
    RUN_TEST(test_json_parser_parse_object_parses_empty_object);
    RUN_TEST(test_json_parser_parse_object_parses_single_member);
    RUN_TEST(test_json_parser_parse_object_parses_multiple_members);
    RUN_TEST(test_json_parser_parse_object_parses_nested_object);

    /* json_parser_parse_array tests */
    RUN_TEST(test_json_parser_parse_array_parses_empty_array);
    RUN_TEST(test_json_parser_parse_array_parses_single_element);
    RUN_TEST(test_json_parser_parse_array_parses_multiple_elements);
    RUN_TEST(test_json_parser_parse_array_parses_nested_array);

    /* json_parser_parse_string tests */
    RUN_TEST(test_json_parser_parse_string_parses_simple_string);
    RUN_TEST(test_json_parser_parse_string_parses_empty_string);
    RUN_TEST(test_json_parser_parse_string_parses_string_with_escape);

    /* json_parser_parse_number tests */
    RUN_TEST(test_json_parser_parse_number_parses_integer);
    RUN_TEST(test_json_parser_parse_number_parses_decimal);
    RUN_TEST(test_json_parser_parse_number_parses_negative);
    RUN_TEST(test_json_parser_parse_number_parses_zero);

    /* json_parser_parse_true/false/null tests */
    RUN_TEST(test_json_parser_parse_true_returns_json_true);
    RUN_TEST(test_json_parser_parse_false_returns_json_false);
    RUN_TEST(test_json_parser_parse_null_returns_json_null);

    /* json_parser_expect tests */
    RUN_TEST(test_json_parser_expect_returns_true_for_matching_token);
    RUN_TEST(test_json_parser_expect_returns_false_for_mismatched_token);

    /* json_writer_init tests */
    RUN_TEST(test_json_writer_init_sets_length_to_zero);

    /* json_writer_expand tests */
    RUN_TEST(test_json_writer_expand_increases_capacity);
    RUN_TEST(test_json_writer_expand_preserves_existing_content);

    /* json_writer_append_char tests */
    RUN_TEST(test_json_writer_append_char_appends_single_char);
    RUN_TEST(test_json_writer_append_char_updates_length);
    RUN_TEST(test_json_writer_append_char_null_terminates);

    /* json_writer_append_string tests */
    RUN_TEST(test_json_writer_append_string_appends_string);
    RUN_TEST(test_json_writer_append_string_updates_length);
    RUN_TEST(test_json_writer_append_string_handles_empty_string);

    /* json_writer_release tests */
    RUN_TEST(test_json_writer_release_returns_buffer);
    RUN_TEST(test_json_writer_release_null_terminates);
    RUN_TEST(test_json_writer_release_resets_writer_state);

    /* json_serialize_string tests */
    RUN_TEST(test_json_serialize_string_wraps_in_quotes);
    RUN_TEST(test_json_serialize_string_escapes_special_chars);
    RUN_TEST(test_json_serialize_string_handles_empty_string);

    /* json_serialize_number tests */
    RUN_TEST(test_json_serialize_number_serializes_integer);
    RUN_TEST(test_json_serialize_number_serializes_decimal);
    RUN_TEST(test_json_serialize_number_serializes_negative);
    RUN_TEST(test_json_serialize_number_serializes_zero);

    /* json_serialize_value tests */
    RUN_TEST(test_json_serialize_value_serializes_string);
    RUN_TEST(test_json_serialize_value_serializes_number);
    RUN_TEST(test_json_serialize_value_serializes_true);
    RUN_TEST(test_json_serialize_value_serializes_false);
    RUN_TEST(test_json_serialize_value_serializes_null);

    /* json_serialize_object tests */
    RUN_TEST(test_json_serialize_object_serializes_empty_object);
    RUN_TEST(test_json_serialize_object_serializes_single_member);
    RUN_TEST(test_json_serialize_object_serializes_multiple_members);

    /* json_serialize_array tests */
    RUN_TEST(test_json_serialize_array_serializes_empty_array);
    RUN_TEST(test_json_serialize_array_serializes_single_element);
    RUN_TEST(test_json_serialize_array_serializes_multiple_elements);

    /* json_serialize tests */
    RUN_TEST(test_json_serialize_serializes_object);
    RUN_TEST(test_json_serialize_serializes_array);
    RUN_TEST(test_json_serialize_serializes_primitive);

    /* integration tests */
    RUN_TEST(test_integration_parse_serialize_object);
    RUN_TEST(test_integration_parse_serialize_array);
    RUN_TEST(test_integration_parse_serialize_nested);
    RUN_TEST(test_integration_parse_serialize_primitives);

    return UNITY_END();
}
