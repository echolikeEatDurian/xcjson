//
// Created by echo on 2026/6/13.
//

#ifndef CJSON_FORK_TEST_CJSON_PARSER_H
#define CJSON_FORK_TEST_CJSON_PARSER_H

/* ================================================================
 * 1. json_parser_init 测试
 * ================================================================ */
void test_json_parser_init_sets_scanner(void);
void test_json_parser_init_sets_has_error_to_false(void);
void test_json_parser_init_sets_error_msg_to_null(void);

/* ================================================================
 * 2. json_parser_parse - 完整解析测试
 * ================================================================ */
void test_json_parser_parse_parses_null(void);
void test_json_parser_parse_parses_true(void);
void test_json_parser_parse_parses_false(void);
void test_json_parser_parse_parses_string(void);
void test_json_parser_parse_parses_number(void);
void test_json_parser_parse_parses_object(void);
void test_json_parser_parse_parses_array(void);

/* ================================================================
 * 3. json_parser_parse_value 测试
 * ================================================================ */
void test_json_parser_parse_value_parses_null(void);
void test_json_parser_parse_value_parses_true(void);
void test_json_parser_parse_value_parses_false(void);
void test_json_parser_parse_value_parses_string(void);
void test_json_parser_parse_value_parses_number(void);
void test_json_parser_parse_value_parses_object(void);
void test_json_parser_parse_value_parses_array(void);

/* ================================================================
 * 4. json_parser_parse_object 测试
 * ================================================================ */
void test_json_parser_parse_object_parses_empty_object(void);
void test_json_parser_parse_object_parses_single_member(void);
void test_json_parser_parse_object_parses_multiple_members(void);
void test_json_parser_parse_object_parses_nested_object(void);

/* ================================================================
 * 5. json_parser_parse_array 测试
 * ================================================================ */
void test_json_parser_parse_array_parses_empty_array(void);
void test_json_parser_parse_array_parses_single_element(void);
void test_json_parser_parse_array_parses_multiple_elements(void);
void test_json_parser_parse_array_parses_nested_array(void);

/* ================================================================
 * 6. json_parser_parse_string 测试
 * ================================================================ */
void test_json_parser_parse_string_parses_simple_string(void);
void test_json_parser_parse_string_parses_empty_string(void);
void test_json_parser_parse_string_parses_string_with_escape(void);

/* ================================================================
 * 7. json_parser_parse_number 测试
 * ================================================================ */
void test_json_parser_parse_number_parses_integer(void);
void test_json_parser_parse_number_parses_decimal(void);
void test_json_parser_parse_number_parses_negative(void);
void test_json_parser_parse_number_parses_zero(void);

/* ================================================================
 * 8. json_parser_parse_true/false/null 测试
 * ================================================================ */
void test_json_parser_parse_true_returns_json_true(void);
void test_json_parser_parse_false_returns_json_false(void);
void test_json_parser_parse_null_returns_json_null(void);

/* ================================================================
 * 9. json_parser_expect 测试
 * ================================================================ */
void test_json_parser_expect_returns_true_for_matching_token(void);
void test_json_parser_expect_returns_false_for_mismatched_token(void);

/* ================================================================
 * 10. json_writer_init 测试
 * ================================================================ */
void test_json_writer_init_sets_length_to_zero(void);

/* ================================================================
 * 11. json_writer_expand 测试
 * ================================================================ */
void test_json_writer_expand_increases_capacity(void);
void test_json_writer_expand_preserves_existing_content(void);

/* ================================================================
 * 12. json_writer_append_char 测试
 * ================================================================ */
void test_json_writer_append_char_appends_single_char(void);
void test_json_writer_append_char_updates_length(void);
void test_json_writer_append_char_null_terminates(void);

/* ================================================================
 * 13. json_writer_append_string 测试
 * ================================================================ */
void test_json_writer_append_string_appends_string(void);
void test_json_writer_append_string_updates_length(void);
void test_json_writer_append_string_handles_empty_string(void);

/* ================================================================
 * 14. json_writer_release 测试
 * ================================================================ */
void test_json_writer_release_returns_buffer(void);
void test_json_writer_release_null_terminates(void);
void test_json_writer_release_resets_writer_state(void);

/* ================================================================
 * 15. json_serialize_string 测试
 * ================================================================ */
void test_json_serialize_string_wraps_in_quotes(void);
void test_json_serialize_string_escapes_special_chars(void);
void test_json_serialize_string_handles_empty_string(void);

/* ================================================================
 * 16. json_serialize_number 测试
 * ================================================================ */
void test_json_serialize_number_serializes_integer(void);
void test_json_serialize_number_serializes_decimal(void);
void test_json_serialize_number_serializes_negative(void);
void test_json_serialize_number_serializes_zero(void);

/* ================================================================
 * 17. json_serialize_value 测试
 * ================================================================ */
void test_json_serialize_value_serializes_string(void);
void test_json_serialize_value_serializes_number(void);
void test_json_serialize_value_serializes_true(void);
void test_json_serialize_value_serializes_false(void);
void test_json_serialize_value_serializes_null(void);

/* ================================================================
 * 18. json_serialize_object 测试
 * ================================================================ */
void test_json_serialize_object_serializes_empty_object(void);
void test_json_serialize_object_serializes_single_member(void);
void test_json_serialize_object_serializes_multiple_members(void);

/* ================================================================
 * 19. json_serialize_array 测试
 * ================================================================ */
void test_json_serialize_array_serializes_empty_array(void);
void test_json_serialize_array_serializes_single_element(void);
void test_json_serialize_array_serializes_multiple_elements(void);

/* ================================================================
 * 20. json_serialize - 完整序列化测试
 * ================================================================ */
void test_json_serialize_serializes_object(void);
void test_json_serialize_serializes_array(void);
void test_json_serialize_serializes_primitive(void);

/* ================================================================
 * 21. 集成测试 - Parse/Serialize 往返测试
 * ================================================================ */
void test_integration_parse_serialize_object(void);
void test_integration_parse_serialize_array(void);
void test_integration_parse_serialize_nested(void);
void test_integration_parse_serialize_primitives(void);

#endif //CJSON_FORK_TEST_CJSON_PARSER_H
