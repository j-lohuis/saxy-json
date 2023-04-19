# saxy-json
C++ SAX JSON API Library

## Getting Started

To use this library, you simply add the file [saxy-json.hpp](include/saxy-json.hpp) to your project and include it.


## Examples

### Writer

```c++
#include <saxy_json.hpp>

namespace json = saxy_json;

json::Writer writer{std::cout};

writer.StartObject(); // Root object

writer.KeyValue("name", "John Doe");

writer.KeyValue("age", 35);

writer.Key("address");
writer.StartObject(); // Address object

writer.KeyValue("street", "123 Main St");
writer.KeyValue("city", "Springfield");
writer.KeyValue("state", "IL");
writer.KeyValue("zip_code", "62704");

writer.FinishObject(); // End of address object

writer.Key("phone_numbers");
writer.StartArray(); // Phone numbers array

writer.StartObject(); // First phone number object

writer.KeyValue("type", "home");
writer.KeyValue("number", "555-5678");

writer.FinishObject(); // End of first phone number object

writer.StartObject(); // Second phone number object

writer.KeyValue("type", "work");
writer.KeyValue("number", "555-8765");

writer.FinishObject(); // End of second phone number object

writer.FinishArray(); // End of phone numbers array

writer.KeyValue("email", "john.doe@eample.com");

writer.FinishObject(); // End of root object

std::cout << std::endl;

/*
*/
```

Result:

```json
{"name":"John Doe","age":35,"address":{"street":"123 Main St","city":"Springfield","state":"IL","zip_code":"62704"},"phone_numbers":[{"type":"home","number":"555-1234"},{"type":"work","number":"555-5678"}],"email":"john.doe@example.com"}
```

### PrettyWriter

```c++
#include <saxy_json.hpp>

namespace json = saxy_json;

json::PrettyWriter writer{std::cout, 4};

// same as above ...
```

Result:

```json
{
    "name": "John Doe",
    "age": 35,
    "address": {
        "street": "123 Main St",
        "city": "Springfield",
        "state": "IL",
        "zip_code": "62704"
    },
    "phone_numbers": [
        {
            "type": "home",
            "number": "555-1234"
        },
        {
            "type": "work",
            "number": "555-5678"
        }
    ],
    "email": "john.doe@example.com"
}
```


## Test results

`i_*.json` may be accepted or result in an error.

`n_*.json` must result in an error.

`y_*.json` must be accepted.


| File in `test/json_files/test_parsing`                         | Result (Accepted/Error message)                                                                                                                                                 |
|----------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| i_number_double_huge_neg_exp.json                              | Failed with Could not convert '123456e-789' to float                                                                                                                            |
| i_number_huge_exp.json                                         | Failed with Could not convert '04e00669999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999969999999006' to float |
| i_number_neg_int_huge_exp.json                                 | Accepted                                                                                                                                                                        |
| i_number_pos_double_huge_exp.json                              | Failed with Could not convert '15e+9999' to float                                                                                                                               |
| i_number_real_neg_overflow.json                                | Accepted                                                                                                                                                                        |
| i_number_real_pos_overflow.json                                | Accepted                                                                                                                                                                        |
| i_number_real_underflow.json                                   | Accepted                                                                                                                                                                        |
| i_number_too_big_neg_int.json                                  | Failed with Could not convert '-123123123123123123123123123123' to integer                                                                                                      |
| i_number_too_big_pos_int.json                                  | Failed with Could not convert '100000000000000000000' to integer                                                                                                                |
| i_number_very_big_negative_int.json                            | Failed with Could not convert '-237462374673276894279832749832423479823246327846' to integer                                                                                    |
| i_object_key_lone_2nd_surrogate.json                           | Accepted                                                                                                                                                                        |
| i_string_1st_surrogate_but_2nd_missing.json                    | Accepted                                                                                                                                                                        |
| i_string_1st_valid_surrogate_2nd_invalid.json                  | Accepted                                                                                                                                                                        |
| i_string_incomplete_surrogate_and_escape_valid.json            | Accepted                                                                                                                                                                        |
| i_string_incomplete_surrogate_pair.json                        | Accepted                                                                                                                                                                        |
| i_string_incomplete_surrogates_escape_valid.json               | Accepted                                                                                                                                                                        |
| i_string_invalid_lonely_surrogate.json                         | Accepted                                                                                                                                                                        |
| i_string_invalid_surrogate.json                                | Accepted                                                                                                                                                                        |
| i_string_invalid_utf-8.json                                    | Accepted                                                                                                                                                                        |
| i_string_inverted_surrogates_U+1D11E.json                      | Accepted                                                                                                                                                                        |
| i_string_iso_latin_1.json                                      | Accepted                                                                                                                                                                        |
| i_string_lone_second_surrogate.json                            | Accepted                                                                                                                                                                        |
| i_string_lone_utf8_continuation_byte.json                      | Accepted                                                                                                                                                                        |
| i_string_not_in_unicode_range.json                             | Accepted                                                                                                                                                                        |
| i_string_overlong_sequence_2_bytes.json                        | Accepted                                                                                                                                                                        |
| i_string_overlong_sequence_6_bytes.json                        | Accepted                                                                                                                                                                        |
| i_string_overlong_sequence_6_bytes_null.json                   | Accepted                                                                                                                                                                        |
| i_string_truncated-utf-8.json                                  | Accepted                                                                                                                                                                        |
| i_string_utf16BE_no_BOM.json                                   | Failed with Unexpected character '                                                                                                                                              |
| i_string_utf16LE_no_BOM.json                                   | Failed with Unexpected character '                                                                                                                                              |
| i_string_UTF-16LE_with_BOM.json                                | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| i_string_UTF-8_invalid_sequence.json                           | Accepted                                                                                                                                                                        |
| i_string_UTF8_surrogate_U+D800.json                            | Accepted                                                                                                                                                                        |
| i_structure_500_nested_arrays.json                             | Accepted                                                                                                                                                                        |
| i_structure_UTF-8_BOM_empty_object.json                        | Failed with Unexpected character '\xef' in ParseValue                                                                                                                           |
| n_array_1_true_without_comma.json                              | Failed with Unexpected char 't'                                                                                                                                                 |
| n_array_a_invalid_utf8.json                                    | Failed with Unexpected character 'a' in ParseValue                                                                                                                              |
| n_array_colon_instead_of_comma.json                            | Failed with Unexpected char ':'                                                                                                                                                 |
| n_array_comma_after_close.json                                 | Accepted                                                                                                                                                                        |
| n_array_comma_and_number.json                                  | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_double_comma.json                                      | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_double_extra_comma.json                                | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_extra_close.json                                       | Accepted                                                                                                                                                                        |
| n_array_extra_comma.json                                       | Failed with Unexpected character ']' in ParseValue                                                                                                                              |
| n_array_incomplete_invalid_value.json                          | Failed with Unexpected character 'x' in ParseValue                                                                                                                              |
| n_array_incomplete.json                                        | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_array_inner_array_no_comma.json                              | Failed with Unexpected char '['                                                                                                                                                 |
| n_array_invalid_utf8.json                                      | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_array_items_separated_by_semicolon.json                      | Failed with Unexpected char ':'                                                                                                                                                 |
| n_array_just_comma.json                                        | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_just_minus.json                                        | Failed with Could not convert '-' to integer                                                                                                                                    |
| n_array_missing_value.json                                     | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_newlines_unclosed.json                                 | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_array_number_and_comma.json                                  | Failed with Unexpected character ']' in ParseValue                                                                                                                              |
| n_array_number_and_several_commas.json                         | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_array_spaces_vertical_tab_formfeed.json                      | Failed with Unexpected char '\'                                                                                                                                                 |
| n_array_star_inside.json                                       | Failed with Unexpected character '*' in ParseValue                                                                                                                              |
| n_array_unclosed.json                                          | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_array_unclosed_trailing_comma.json                           | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_array_unclosed_with_new_lines.json                           | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_array_unclosed_with_object_inside.json                       | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_incomplete_false.json                                        | Failed with Unexpected char ']'                                                                                                                                                 |
| n_incomplete_null.json                                         | Failed with Unexpected char ']'                                                                                                                                                 |
| n_incomplete_true.json                                         | Failed with Unexpected char ']'                                                                                                                                                 |
| n_multidigit_number_then_00.json                               | Accepted                                                                                                                                                                        |
| n_number_0.1.2.json                                            | Failed with Unexpected char '.'                                                                                                                                                 |
| n_number_-01.json                                              | Accepted                                                                                                                                                                        |
| n_number_0.3e+.json                                            | Accepted                                                                                                                                                                        |
| n_number_0.3e.json                                             | Accepted                                                                                                                                                                        |
| n_number_0_capital_E+.json                                     | Accepted                                                                                                                                                                        |
| n_number_0_capital_E.json                                      | Accepted                                                                                                                                                                        |
| n_number_0.e1.json                                             | Accepted                                                                                                                                                                        |
| n_number_0e+.json                                              | Accepted                                                                                                                                                                        |
| n_number_0e.json                                               | Accepted                                                                                                                                                                        |
| n_number_1_000.json                                            | Failed with Unexpected char '0'                                                                                                                                                 |
| n_number_1.0e+.json                                            | Accepted                                                                                                                                                                        |
| n_number_1.0e-.json                                            | Accepted                                                                                                                                                                        |
| n_number_1.0e.json                                             | Accepted                                                                                                                                                                        |
| n_number_-1.0..json                                            | Failed with Unexpected char '.'                                                                                                                                                 |
| n_number_1eE2.json                                             | Accepted                                                                                                                                                                        |
| n_number_+1.json                                               | Failed with Unexpected character '+' in ParseValue                                                                                                                              |
| n_number_.-1.json                                              | Failed with Unexpected character '.' in ParseValue                                                                                                                              |
| n_number_2.e+3.json                                            | Accepted                                                                                                                                                                        |
| n_number_2.e-3.json                                            | Accepted                                                                                                                                                                        |
| n_number_2.e3.json                                             | Accepted                                                                                                                                                                        |
| n_number_.2e-3.json                                            | Failed with Unexpected character '.' in ParseValue                                                                                                                              |
| n_number_-2..json                                              | Accepted                                                                                                                                                                        |
| n_number_9.e+.json                                             | Accepted                                                                                                                                                                        |
| n_number_expression.json                                       | Failed with Unexpected char '+'                                                                                                                                                 |
| n_number_hex_1_digit.json                                      | Failed with Unexpected char 'x'                                                                                                                                                 |
| n_number_hex_2_digits.json                                     | Failed with Unexpected char 'x'                                                                                                                                                 |
| n_number_infinity.json                                         | Failed with Unexpected character 'I' in ParseValue                                                                                                                              |
| n_number_+Inf.json                                             | Failed with Unexpected character '+' in ParseValue                                                                                                                              |
| n_number_Inf.json                                              | Failed with Unexpected character 'I' in ParseValue                                                                                                                              |
| n_number_invalid+-.json                                        | Accepted                                                                                                                                                                        |
| n_number_invalid-negative-real.json                            | Failed with Unexpected char 'f'                                                                                                                                                 |
| n_number_invalid-utf-8-in-bigger-int.json                      | Failed with Unexpected char '\xe5'                                                                                                                                              |
| n_number_invalid-utf-8-in-exponent.json                        | Failed with Unexpected char '\xe5'                                                                                                                                              |
| n_number_invalid-utf-8-in-int.json                             | Failed with Unexpected char '\xe5'                                                                                                                                              |
| n_number_++.json                                               | Failed with Unexpected character '+' in ParseValue                                                                                                                              |
| n_number_minus_infinity.json                                   | Failed with Could not convert '-' to integer                                                                                                                                    |
| n_number_minus_sign_with_trailing_garbage.json                 | Failed with Could not convert '-' to integer                                                                                                                                    |
| n_number_minus_space_1.json                                    | Failed with Could not convert '-' to integer                                                                                                                                    |
| n_number_-NaN.json                                             | Failed with Could not convert '-' to integer                                                                                                                                    |
| n_number_NaN.json                                              | Failed with Unexpected character 'N' in ParseValue                                                                                                                              |
| n_number_neg_int_starting_with_zero.json                       | Accepted                                                                                                                                                                        |
| n_number_neg_real_without_int_part.json                        | Accepted                                                                                                                                                                        |
| n_number_neg_with_garbage_at_end.json                          | Failed with Unexpected char 'x'                                                                                                                                                 |
| n_number_real_garbage_after_e.json                             | Failed with Unexpected char 'a'                                                                                                                                                 |
| n_number_real_with_invalid_utf8_after_e.json                   | Failed with Unexpected char '\xe5'                                                                                                                                              |
| n_number_real_without_fractional_part.json                     | Accepted                                                                                                                                                                        |
| n_number_starting_with_dot.json                                | Failed with Unexpected character '.' in ParseValue                                                                                                                              |
| n_number_U+FF11_fullwidth_digit_one.json                       | Failed with Unexpected character '\xef' in ParseValue                                                                                                                           |
| n_number_with_alpha_char.json                                  | Failed with Unexpected char 'H'                                                                                                                                                 |
| n_number_with_alpha.json                                       | Failed with Unexpected char 'a'                                                                                                                                                 |
| n_number_with_leading_zero.json                                | Accepted                                                                                                                                                                        |
| n_object_bad_value.json                                        | Failed with Unexpected char 't'                                                                                                                                                 |
| n_object_bracket_key.json                                      | Failed with Unexpected char '['                                                                                                                                                 |
| n_object_comma_instead_of_colon.json                           | Failed with Unexpected char ','                                                                                                                                                 |
| n_object_double_colon.json                                     | Failed with Unexpected character ':' in ParseValue                                                                                                                              |
| n_object_emoji.json                                            | Failed with Unexpected char '\xf0'                                                                                                                                              |
| n_object_garbage_at_end.json                                   | Failed with Unexpected char '1'                                                                                                                                                 |
| n_object_key_with_single_quotes.json                           | Failed with Unexpected char 'k'                                                                                                                                                 |
| n_object_lone_continuation_byte_in_key_and_trailing_comma.json | Accepted                                                                                                                                                                        |
| n_object_missing_colon.json                                    | Failed with Unexpected char 'b'                                                                                                                                                 |
| n_object_missing_key.json                                      | Failed with Unexpected char ':'                                                                                                                                                 |
| n_object_missing_semicolon.json                                | Failed with Unexpected char '"'                                                                                                                                                 |
| n_object_missing_value.json                                    | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_object_no-colon.json                                         | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_object_non_string_key_but_huge_number_instead.json           | Failed with Unexpected char '9'                                                                                                                                                 |
| n_object_non_string_key.json                                   | Failed with Unexpected char '1'                                                                                                                                                 |
| n_object_repeated_null_null.json                               | Failed with Unexpected char 'n'                                                                                                                                                 |
| n_object_several_trailing_commas.json                          | Failed with Unexpected char ','                                                                                                                                                 |
| n_object_single_quote.json                                     | Failed with Unexpected char '''                                                                                                                                                 |
| n_object_trailing_comma.json                                   | Accepted                                                                                                                                                                        |
| n_object_trailing_comment.json                                 | Accepted                                                                                                                                                                        |
| n_object_trailing_comment_open.json                            | Accepted                                                                                                                                                                        |
| n_object_trailing_comment_slash_open_incomplete.json           | Accepted                                                                                                                                                                        |
| n_object_trailing_comment_slash_open.json                      | Accepted                                                                                                                                                                        |
| n_object_two_commas_in_a_row.json                              | Failed with Unexpected char ','                                                                                                                                                 |
| n_object_unquoted_key.json                                     | Failed with Unexpected char 'a'                                                                                                                                                 |
| n_object_unterminated-value.json                               | Failed with Unexpected EOF                                                                                                                                                      |
| n_object_with_single_string.json                               | Failed with Unexpected char '}'                                                                                                                                                 |
| n_object_with_trailing_garbage.json                            | Accepted                                                                                                                                                                        |
| n_single_space.json                                            | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_string_1_surrogate_then_escape.json                          | Failed with Unexpected EOF                                                                                                                                                      |
| n_string_1_surrogate_then_escape_u1.json                       | Failed with Invalid hex character '"'                                                                                                                                           |
| n_string_1_surrogate_then_escape_u1x.json                      | Failed with Invalid hex character 'x'                                                                                                                                           |
| n_string_1_surrogate_then_escape_u.json                        | Failed with Invalid hex character '"'                                                                                                                                           |
| n_string_accentuated_char_no_quotes.json                       | Failed with Unexpected character '\xc3' in ParseValue                                                                                                                           |
| n_string_backslash_00.json                                     | Failed with Invalid escape character '                                                                                                                                          |
| n_string_escaped_backslash_bad.json                            | Failed with Unexpected EOF                                                                                                                                                      |
| n_string_escaped_ctrl_char_tab.json                            | Failed with Invalid escape character '	'                                                                                                                                        |
| n_string_escaped_emoji.json                                    | Failed with Invalid escape character '\xf0'                                                                                                                                     |
| n_string_escape_x.json                                         | Failed with Invalid escape character 'x'                                                                                                                                        |
| n_string_incomplete_escaped_character.json                     | Failed with Invalid hex character '"'                                                                                                                                           |
| n_string_incomplete_escape.json                                | Failed with Unexpected EOF                                                                                                                                                      |
| n_string_incomplete_surrogate_escape_invalid.json              | Failed with Invalid escape character 'x'                                                                                                                                        |
| n_string_incomplete_surrogate.json                             | Failed with Invalid hex character '"'                                                                                                                                           |
| n_string_invalid_backslash_esc.json                            | Failed with Invalid escape character 'a'                                                                                                                                        |
| n_string_invalid_unicode_escape.json                           | Failed with Invalid hex character 'q'                                                                                                                                           |
| n_string_invalid_utf8_after_escape.json                        | Failed with Invalid escape character '\xe5'                                                                                                                                     |
| n_string_invalid-utf-8-in-escape.json                          | Failed with Invalid hex character '\xe5'                                                                                                                                        |
| n_string_leading_uescaped_thinspace.json                       | Failed with Unexpected character '\' in ParseValue                                                                                                                              |
| n_string_no_quotes_with_bad_escape.json                        | Failed with Unexpected character '\' in ParseValue                                                                                                                              |
| n_string_single_doublequote.json                               | Failed with Unexpected EOF                                                                                                                                                      |
| n_string_single_quote.json                                     | Failed with Unexpected character ''' in ParseValue                                                                                                                              |
| n_string_single_string_no_double_quotes.json                   | Failed with Unexpected character 'a' in ParseValue                                                                                                                              |
| n_string_start_escape_unclosed.json                            | Failed with Invalid escape character '\xff'                                                                                                                                     |
| n_string_unescaped_ctrl_char.json                              | Accepted                                                                                                                                                                        |
| n_string_unescaped_newline.json                                | Accepted                                                                                                                                                                        |
| n_string_unescaped_tab.json                                    | Accepted                                                                                                                                                                        |
| n_string_unicode_CapitalU.json                                 | Failed with Invalid escape character 'U'                                                                                                                                        |
| n_string_with_trailing_garbage.json                            | Accepted                                                                                                                                                                        |
| n_structure_angle_bracket_..json                               | Failed with Unexpected character '<' in ParseValue                                                                                                                              |
| n_structure_angle_bracket_null.json                            | Failed with Unexpected character '<' in ParseValue                                                                                                                              |
| n_structure_array_trailing_garbage.json                        | Accepted                                                                                                                                                                        |
| n_structure_array_with_extra_array_close.json                  | Accepted                                                                                                                                                                        |
| n_structure_array_with_unclosed_string.json                    | Failed with Unexpected EOF                                                                                                                                                      |
| n_structure_ascii-unicode-identifier.json                      | Failed with Unexpected character 'a' in ParseValue                                                                                                                              |
| n_structure_capitalized_True.json                              | Failed with Unexpected character 'T' in ParseValue                                                                                                                              |
| n_structure_close_unopened_array.json                          | Accepted                                                                                                                                                                        |
| n_structure_comma_instead_of_closing_brace.json                | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_double_array.json                                  | Accepted                                                                                                                                                                        |
| n_structure_end_array.json                                     | Failed with Unexpected character ']' in ParseValue                                                                                                                              |
| n_structure_incomplete_UTF8_BOM.json                           | Failed with Unexpected character '\xef' in ParseValue                                                                                                                           |
| n_structure_lone-invalid-utf-8.json                            | Failed with Unexpected character '\xe5' in ParseValue                                                                                                                           |
| n_structure_lone-open-bracket.json                             | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_structure_no_data.json                                       | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_structure_null-byte-outside-string.json                      | Failed with Unexpected character '                                                                                                                                              |
| n_structure_number_with_trailing_garbage.json                  | Accepted                                                                                                                                                                        |
| n_structure_object_followed_by_closing_object.json             | Accepted                                                                                                                                                                        |
| n_structure_object_unclosed_no_value.json                      | Failed with Unexpected character '\xff' in ParseValue                                                                                                                           |
| n_structure_object_with_comment.json                           | Failed with Unexpected character '/' in ParseValue                                                                                                                              |
| n_structure_object_with_trailing_garbage.json                  | Accepted                                                                                                                                                                        |
| n_structure_open_array_apostrophe.json                         | Failed with Unexpected character ''' in ParseValue                                                                                                                              |
| n_structure_open_array_comma.json                              | Failed with Unexpected character ',' in ParseValue                                                                                                                              |
| n_structure_open_array_open_object.json                        | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_open_array_open_string.json                        | Failed with Unexpected EOF                                                                                                                                                      |
| n_structure_open_array_string.json                             | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_open_object_close_array.json                       | Failed with Unexpected char ']'                                                                                                                                                 |
| n_structure_open_object_comma.json                             | Failed with Unexpected char ','                                                                                                                                                 |
| n_structure_open_object.json                                   | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_open_object_open_array.json                        | Failed with Unexpected char '['                                                                                                                                                 |
| n_structure_open_object_open_string.json                       | Failed with Unexpected EOF                                                                                                                                                      |
| n_structure_open_object_string_with_apostrophes.json           | Failed with Unexpected char '''                                                                                                                                                 |
| n_structure_open_open.json                                     | Failed with Invalid escape character '{'                                                                                                                                        |
| n_structure_single_eacute.json                                 | Failed with Unexpected character '\xe9' in ParseValue                                                                                                                           |
| n_structure_single_star.json                                   | Failed with Unexpected character '*' in ParseValue                                                                                                                              |
| n_structure_trailing_#.json                                    | Accepted                                                                                                                                                                        |
| n_structure_U+2060_word_joined.json                            | Failed with Unexpected character '\xe2' in ParseValue                                                                                                                           |
| n_structure_uescaped_LF_before_string.json                     | Failed with Unexpected character '\' in ParseValue                                                                                                                              |
| n_structure_unclosed_array.json                                | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_unclosed_array_partial_null.json                   | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_unclosed_array_unfinished_false.json               | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_unclosed_array_unfinished_true.json                | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_unclosed_object.json                               | Failed with Unexpected char '\xff'                                                                                                                                              |
| n_structure_unicode-identifier.json                            | Failed with Unexpected character '\xc3' in ParseValue                                                                                                                           |
| n_structure_UTF8_BOM_no_data.json                              | Failed with Unexpected character '\xef' in ParseValue                                                                                                                           |
| n_structure_whitespace_formfeed.json                           | Failed with Unexpected character '' in ParseValue                                                                                                                              |
| n_structure_whitespace_U+2060_word_joiner.json                 | Failed with Unexpected character '\xe2' in ParseValue                                                                                                                           |
| y_array_arraysWithSpaces.json                                  | Accepted                                                                                                                                                                        |
| y_array_empty.json                                             | Accepted                                                                                                                                                                        |
| y_array_empty-string.json                                      | Accepted                                                                                                                                                                        |
| y_array_ending_with_newline.json                               | Accepted                                                                                                                                                                        |
| y_array_false.json                                             | Accepted                                                                                                                                                                        |
| y_array_heterogeneous.json                                     | Accepted                                                                                                                                                                        |
| y_array_null.json                                              | Accepted                                                                                                                                                                        |
| y_array_with_1_and_newline.json                                | Accepted                                                                                                                                                                        |
| y_array_with_leading_space.json                                | Accepted                                                                                                                                                                        |
| y_array_with_several_null.json                                 | Accepted                                                                                                                                                                        |
| y_array_with_trailing_space.json                               | Accepted                                                                                                                                                                        |
| y_number_0e+1.json                                             | Accepted                                                                                                                                                                        |
| y_number_0e1.json                                              | Accepted                                                                                                                                                                        |
| y_number_after_space.json                                      | Accepted                                                                                                                                                                        |
| y_number_double_close_to_zero.json                             | Accepted                                                                                                                                                                        |
| y_number_int_with_exp.json                                     | Accepted                                                                                                                                                                        |
| y_number.json                                                  | Accepted                                                                                                                                                                        |
| y_number_minus_zero.json                                       | Accepted                                                                                                                                                                        |
| y_number_negative_int.json                                     | Accepted                                                                                                                                                                        |
| y_number_negative_one.json                                     | Accepted                                                                                                                                                                        |
| y_number_negative_zero.json                                    | Accepted                                                                                                                                                                        |
| y_number_real_capital_e.json                                   | Accepted                                                                                                                                                                        |
| y_number_real_capital_e_neg_exp.json                           | Accepted                                                                                                                                                                        |
| y_number_real_capital_e_pos_exp.json                           | Accepted                                                                                                                                                                        |
| y_number_real_exponent.json                                    | Accepted                                                                                                                                                                        |
| y_number_real_fraction_exponent.json                           | Accepted                                                                                                                                                                        |
| y_number_real_neg_exp.json                                     | Accepted                                                                                                                                                                        |
| y_number_real_pos_exponent.json                                | Accepted                                                                                                                                                                        |
| y_number_simple_int.json                                       | Accepted                                                                                                                                                                        |
| y_number_simple_real.json                                      | Accepted                                                                                                                                                                        |
| y_object_basic.json                                            | Accepted                                                                                                                                                                        |
| y_object_duplicated_key_and_value.json                         | Accepted                                                                                                                                                                        |
| y_object_duplicated_key.json                                   | Accepted                                                                                                                                                                        |
| y_object_empty.json                                            | Accepted                                                                                                                                                                        |
| y_object_empty_key.json                                        | Accepted                                                                                                                                                                        |
| y_object_escaped_null_in_key.json                              | Accepted                                                                                                                                                                        |
| y_object_extreme_numbers.json                                  | Accepted                                                                                                                                                                        |
| y_object.json                                                  | Accepted                                                                                                                                                                        |
| y_object_long_strings.json                                     | Accepted                                                                                                                                                                        |
| y_object_simple.json                                           | Accepted                                                                                                                                                                        |
| y_object_string_unicode.json                                   | Accepted                                                                                                                                                                        |
| y_object_with_newlines.json                                    | Accepted                                                                                                                                                                        |
| y_string_1_2_3_bytes_UTF-8_sequences.json                      | Accepted                                                                                                                                                                        |
| y_string_accepted_surrogate_pair.json                          | Accepted                                                                                                                                                                        |
| y_string_accepted_surrogate_pairs.json                         | Accepted                                                                                                                                                                        |
| y_string_allowed_escapes.json                                  | Accepted                                                                                                                                                                        |
| y_string_backslash_and_u_escaped_zero.json                     | Accepted                                                                                                                                                                        |
| y_string_backslash_doublequotes.json                           | Accepted                                                                                                                                                                        |
| y_string_comments.json                                         | Accepted                                                                                                                                                                        |
| y_string_double_escape_a.json                                  | Accepted                                                                                                                                                                        |
| y_string_double_escape_n.json                                  | Accepted                                                                                                                                                                        |
| y_string_escaped_control_character.json                        | Accepted                                                                                                                                                                        |
| y_string_escaped_noncharacter.json                             | Accepted                                                                                                                                                                        |
| y_string_in_array.json                                         | Accepted                                                                                                                                                                        |
| y_string_in_array_with_leading_space.json                      | Accepted                                                                                                                                                                        |
| y_string_last_surrogates_1_and_2.json                          | Accepted                                                                                                                                                                        |
| y_string_nbsp_uescaped.json                                    | Accepted                                                                                                                                                                        |
| y_string_nonCharacterInUTF-8_U+10FFFF.json                     | Accepted                                                                                                                                                                        |
| y_string_nonCharacterInUTF-8_U+FFFF.json                       | Accepted                                                                                                                                                                        |
| y_string_null_escape.json                                      | Accepted                                                                                                                                                                        |
| y_string_one-byte-utf-8.json                                   | Accepted                                                                                                                                                                        |
| y_string_pi.json                                               | Accepted                                                                                                                                                                        |
| y_string_reservedCharacterInUTF-8_U+1BFFF.json                 | Accepted                                                                                                                                                                        |
| y_string_simple_ascii.json                                     | Accepted                                                                                                                                                                        |
| y_string_space.json                                            | Accepted                                                                                                                                                                        |
| y_string_surrogates_U+1D11E_MUSICAL_SYMBOL_G_CLEF.json         | Accepted                                                                                                                                                                        |
| y_string_three-byte-utf-8.json                                 | Accepted                                                                                                                                                                        |
| y_string_two-byte-utf-8.json                                   | Accepted                                                                                                                                                                        |
| y_string_u+2028_line_sep.json                                  | Accepted                                                                                                                                                                        |
| y_string_u+2029_par_sep.json                                   | Accepted                                                                                                                                                                        |
| y_string_uescaped_newline.json                                 | Accepted                                                                                                                                                                        |
| y_string_uEscape.json                                          | Accepted                                                                                                                                                                        |
| y_string_unescaped_char_delete.json                            | Accepted                                                                                                                                                                        |
| y_string_unicode_2.json                                        | Accepted                                                                                                                                                                        |
| y_string_unicodeEscapedBackslash.json                          | Accepted                                                                                                                                                                        |
| y_string_unicode_escaped_double_quote.json                     | Accepted                                                                                                                                                                        |
| y_string_unicode.json                                          | Accepted                                                                                                                                                                        |
| y_string_unicode_U+10FFFE_nonchar.json                         | Accepted                                                                                                                                                                        |
| y_string_unicode_U+1FFFE_nonchar.json                          | Accepted                                                                                                                                                                        |
| y_string_unicode_U+200B_ZERO_WIDTH_SPACE.json                  | Accepted                                                                                                                                                                        |
| y_string_unicode_U+2064_invisible_plus.json                    | Accepted                                                                                                                                                                        |
| y_string_unicode_U+FDD0_nonchar.json                           | Accepted                                                                                                                                                                        |
| y_string_unicode_U+FFFE_nonchar.json                           | Accepted                                                                                                                                                                        |
| y_string_utf8.json                                             | Accepted                                                                                                                                                                        |
| y_string_with_del_character.json                               | Accepted                                                                                                                                                                        |
| y_structure_lonely_false.json                                  | Accepted                                                                                                                                                                        |
| y_structure_lonely_int.json                                    | Accepted                                                                                                                                                                        |
| y_structure_lonely_negative_real.json                          | Accepted                                                                                                                                                                        |
| y_structure_lonely_null.json                                   | Accepted                                                                                                                                                                        |
| y_structure_lonely_string.json                                 | Accepted                                                                                                                                                                        |
| y_structure_lonely_true.json                                   | Accepted                                                                                                                                                                        |
| y_structure_string_empty.json                                  | Accepted                                                                                                                                                                        |
| y_structure_trailing_newline.json                              | Accepted                                                                                                                                                                        |
| y_structure_true_in_array.json                                 | Accepted                                                                                                                                                                        |
| y_structure_whitespace_array.json                              | Accepted                                                                                                                                                                        |
