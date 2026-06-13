//
// Created by echo on 2026/6/13.
//

#include "Cjson.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/**
 * 向后走
 * @param json_scanner
 * @return
 */
CJSON_PUBLIC(char) json_scanner_advance(JsonTokenScanner *json_scanner) {
    json_scanner->col++;
    return *json_scanner->current++;
}

/**
 * 当前元素 current指向
 * @param json_scanner
 * @return
 */
CJSON_PUBLIC(char) json_scanner_peek(JsonTokenScanner *json_scanner) {
    return *json_scanner->current;
}

/**
 *
 * 跳过空白
 * @param json_scanner
 */
CJSON_PUBLIC(void) json_scanner_skip_whitespace(JsonTokenScanner *json_scanner) {
    while (*json_scanner->current == '\n'
           || *json_scanner->current == '\r'
           || *json_scanner->current == '\f'
           || *json_scanner->current == '\v'
           || *json_scanner->current == '\t'
           || *json_scanner->current == ' '
    ) {
        if (*json_scanner->current == '\n') {
            json_scanner->col = 0;
            json_scanner->row++;
        }
        json_scanner_advance(json_scanner);
    }
}

/**
 * 创建token
 * @param json_scanner
 * @param token_type
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_new_json_token(JsonTokenScanner *json_scanner, JsonTokenType token_type) {
    struct JsonToken token = {};
    token.token_type = token_type;
    token.col = json_scanner->col;
    token.row = json_scanner->row;

    token.start = json_scanner->start;
    token.length = (int) (json_scanner->current - json_scanner->start);

    char *buffer = (char *) malloc(sizeof(char));
    if (!buffer) {
        token.token_type = JSON_TOKEN_ERROR;
        token.error_msg = "allocate memory failed";
        return token;
    }

    memcpy(buffer, json_scanner->start, token.length);
    buffer[token.length] = '\0';
    token.value_string = buffer;

    if (token_type == JSON_TOKEN_NUMBER) {
        char *endptr;
        errno = 0;
        double value = strtod(token.value_string, &endptr);
        if (errno == ERANGE || endptr == token.value_string) {
            token.token_type = JSON_TOKEN_ERROR;
            token.error_msg = "parse error";
        }
        token.value_number = value;
    }
    return token;
}

/**
 *  尝试解析 number
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_number(JsonTokenScanner *scanner) {
    const char *start = scanner->current;
    // optional minus
    if (json_scanner_peek(scanner) == '-') {
        json_scanner_advance(scanner);
    }

    // integer part
    if (json_scanner_peek(scanner) == '0') {
        json_scanner_advance(scanner);
    } else if (isdigit(json_scanner_peek(scanner))) {
        while (isdigit(json_scanner_peek(scanner))) {
            json_scanner_advance(scanner);
        }
    } else {
        char buf[128];
        snprintf(buf, sizeof(buf),
                 "Invalid number at line %d, col %d\n", scanner->row, scanner->col);

        struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
        token.error_msg = strdup(buf);
        return token;
    }

    // fractional part
    if (json_scanner_peek(scanner) == '.') {
        json_scanner_advance(scanner);
        if (!isdigit(json_scanner_peek(scanner))) {
            char buf[128];
            snprintf(buf, sizeof(buf), "Invalid number: expected digit after '.' at line %d, col %d\n", scanner->row,
                     scanner->col);
            struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
            token.error_msg = strdup(buf);
            return token;
        }
        while (isdigit(json_scanner_peek(scanner))) {
            json_scanner_advance(scanner);
        }
    }

    // exponent part
    if (json_scanner_peek(scanner) == 'e' || json_scanner_peek(scanner) == 'E') {
        json_scanner_advance(scanner);
        if (json_scanner_peek(scanner) == '+' || json_scanner_peek(scanner) == '-') {
            json_scanner_advance(scanner);
        }
        if (!isdigit(json_scanner_peek(scanner))) {
            char buf[128];
            snprintf(buf, sizeof(buf), "Invalid number: expected digit in exponent at line %d, col %d\n", scanner->row,
                     scanner->col);
            struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
            token.error_msg = strdup(buf);
            return token;
        }
        while (isdigit(json_scanner_peek(scanner))) {
            json_scanner_advance(scanner);
        }
    }
    return json_scanner_new_json_token(scanner, JSON_TOKEN_NUMBER);
}

/**
 * 尝试解析string
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_string(JsonTokenScanner *scanner) {
    json_scanner_advance(scanner); // skip opening "
    scanner->start = scanner->current; // start 指向内容开始

    while (json_scanner_peek(scanner) != '\0') {
        char c = json_scanner_peek(scanner);
        if (c == '"') {
            // current 指向结尾引号，此时 start 到 current 正好是内容
            struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_STRING);
            json_scanner_advance(scanner); // 跳过结尾引号，让后续解析继续
            return token;
        }
        if (c == '\\') {
            json_scanner_advance(scanner); // skip '\'
            if (json_scanner_peek(scanner) != '\0') {
                json_scanner_advance(scanner); // skip escaped char
            }
            continue;
        }
        json_scanner_advance(scanner);
    }

    char buf[128];
    snprintf(buf, sizeof(buf),
             "Invalid token: expected 'true' at line %d, col %d\n",
             scanner->row, scanner->col);
    struct JsonToken errorToken = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
    errorToken.error_msg = strdup(buf);
    return errorToken;
}

/**
 * 尝试解析true
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_true(JsonTokenScanner *scanner) {
    // true
    char c1 = scanner->current[1];
    char c2 = scanner->current[2];
    char c3 = scanner->current[3];

    if ((c1 == 'r' || c1 == 'R') &&
        (c2 == 'u' || c2 == 'U') &&
        (c3 == 'e' || c3 == 'E')) {
        json_scanner_advance(scanner); // t
        json_scanner_advance(scanner); // r
        json_scanner_advance(scanner); // u
        json_scanner_advance(scanner); // e
        return json_scanner_new_json_token(scanner, JSON_TOKEN_TRUE);
    }
    char buf[128];
    snprintf(buf, sizeof(buf),
             "Invalid token: expected 'true' at line %d, col %d\n",
             scanner->row, scanner->col);
    struct JsonToken errorToken = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
    errorToken.error_msg = strdup(buf);
    return errorToken;
}

/**
 *  尝试解析false
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_false(JsonTokenScanner *scanner) {
    // false
    char c1 = scanner->current[1];
    char c2 = scanner->current[2];
    char c3 = scanner->current[3];
    char c4 = scanner->current[4];

    if ((c1 == 'a' || c1 == 'A') &&
        (c2 == 'l' || c2 == 'L') &&
        (c3 == 's' || c3 == 'S') &&
        (c4 == 'e' || c4 == 'E')) {
        json_scanner_advance(scanner); // f
        json_scanner_advance(scanner); // a
        json_scanner_advance(scanner); // l
        json_scanner_advance(scanner); // s
        json_scanner_advance(scanner); // e
        return json_scanner_new_json_token(scanner, JSON_TOKEN_FALSE);
    }
    char buf[128];
    snprintf(buf, sizeof(buf), "Invalid token: expected 'false' at line %d, col %d\n", scanner->row, scanner->col);

    struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
    token.error_msg = strdup(buf);
    return token;
}

/**
 *  尝试解析null
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_null(JsonTokenScanner *scanner) {
    // null
    char c1 = scanner->current[1];
    char c2 = scanner->current[2];
    char c3 = scanner->current[3];

    if ((c1 == 'u' || c1 == 'U') &&
        (c2 == 'l' || c2 == 'L') &&
        (c3 == 'l' || c3 == 'L')) {
        json_scanner_advance(scanner); // n
        json_scanner_advance(scanner); // u
        json_scanner_advance(scanner); // l
        json_scanner_advance(scanner); // l
        return json_scanner_new_json_token(scanner, JSON_TOKEN_NULL);
    }

    char buf[128];
    snprintf(buf, sizeof(buf), "Invalid token: expected 'null' at line %d, col %d\n", scanner->row, scanner->col);
    struct JsonToken token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
    token.error_msg = strdup(buf);
    return token;
}

/**
 *  获取下一个token
 * @param scanner
 * @return
 */
CJSON_PUBLIC(struct JsonToken) json_scanner_next_json_token(JsonTokenScanner *scanner) {
    json_scanner_skip_whitespace(scanner);
    scanner->start = scanner->current;

    if (json_scanner_peek(scanner) == '\0') {
        return json_scanner_new_json_token(scanner, JSON_TOKEN_EOF);
    }

    char c = json_scanner_peek(scanner);
    struct JsonToken token = {};
    int success = -1;
    switch (c) {
        case '{':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_L_BRACE);
            break;
        case '}':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_R_BRACE);
            break;
        case '[':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_L_BRACKET);
            break;
        case ']':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_R_BRACKET);
            break;
        case ':':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_COLON);
            break;
        case ',':
            json_scanner_advance(scanner);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_COMMA);
            break;
        case'-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            token = json_scanner_try_parse_number(scanner);
            break;
        case '"':
            token = json_scanner_try_parse_string(scanner);
            break;
        case 't':
        case 'T':
            token = json_scanner_try_parse_true(scanner);
            break;
        case 'F':
        case 'f':
            token = json_scanner_try_parse_false(scanner);
            break;
        case 'N':
        case 'n':
            token = json_scanner_try_parse_null(scanner);
            break;
        default:
            char buf[128];
            snprintf(buf, sizeof(buf), "unexpected Token: '%c' at line %d, col %d\n", c, scanner->row, scanner->col);
            token = json_scanner_new_json_token(scanner, JSON_TOKEN_ERROR);
            token.error_msg = strdup(buf);
    }
    return token;
}

/**
 *  初始化
 * @param scanner
 * @param json
 */
CJSON_PUBLIC(void) json_scanner_init_scanner(JsonTokenScanner *scanner, const char *json) {
    scanner->row = 1;
    scanner->col = 1;
    scanner->start = json;
    scanner->current = json;
}

/**
 * 打印函数
 * @param token
 */
CJSON_PUBLIC(void) json_token_print(struct JsonToken *token) {
    const char *type_names[] = {
        "L_BRACE", // {
        "R_BRACE", // }
        "L_BRACKET", // [
        "R_BRACKET", // ]
        "COMMA", // ,
        "COLON", // :
        "NUMBER", // 数字
        "STRING", // 字符串
        "TRUE", // true
        "FALSE", // false
        "NULL", // null
        "ERROR", // 错误
        "EOF" // 结束
    };

    printf("[Line %-3llu, Col %-3llu] %-12s: ", token->row, token->col, type_names[token->token_type]);

    if (token->token_type == JSON_TOKEN_NUMBER) {
        printf("[%g]\n", token->value_number);
    } else if (token->token_type == JSON_TOKEN_STRING) {
        printf("[%s]\n", token->value_string);
    } else if (token->token_type == JSON_TOKEN_ERROR) {
        printf("[ERROR: %s]\n", token->error_msg ? token->error_msg : "unknown");
    } else {
        printf("[%s]\n", token->value_string);
    }
}

/**
 * tostring方法
 * @param token_type
 * @return
 */
CJSON_PUBLIC(const char *)json_token_to_string(const JsonTokenType token_type) {
    switch (token_type) {
        case JSON_TOKEN_L_BRACE:
            return "{"; //{
        case JSON_TOKEN_R_BRACE:
            return "}"; //}
        case JSON_TOKEN_L_BRACKET:
            return "["; //[
        case JSON_TOKEN_R_BRACKET: //]
            return "]";
        case JSON_TOKEN_COMMA: //,
            return ",";
        case JSON_TOKEN_COLON: //:
            return ":";
        case JSON_TOKEN_NUMBER: //
            return "number";
        case JSON_TOKEN_STRING: //
            return "string";
        case JSON_TOKEN_TRUE: //
            return "true";
        case JSON_TOKEN_FALSE: //
            return "false";
        case JSON_TOKEN_NULL: //
            return "null";
        case JSON_TOKEN_ERROR: //
            return "error";
        case JSON_TOKEN_EOF: //}
            return "eof";
        default:
            return "unknown";
    }
}


/**
 *    JSON    ::= Value
 *    Value   ::= Object | Array | String | Number | true | false | null
 *    Object  ::= '{' MemberList? '}'
 *    MemberList ::= Member (',' Member)*
 *    Member ::= String ':' Value
 *    Array   ::= '[' ValueList? ']'
 *    ValueList ::= Value (',' Value)*
 */
CJSON_PUBLIC(struct Json*) json_parser_parse(JsonParser *parser) {
    return json_parser_parse_value(parser);
}

CJSON_PUBLIC(struct Json*) json_parser_parse_value(JsonParser *parser) {
    struct Json *json = NULL;
    switch (parser->currentToken.token_type) {
        case (JSON_TOKEN_L_BRACE):
            json = json_parser_parse_object(parser);
            break;
        case (JSON_TOKEN_L_BRACKET):
            json = json_parser_parse_array(parser);
            break;
        case (JSON_TOKEN_STRING):
            json = json_parser_parse_string(parser);
            break;
        case (JSON_TOKEN_NUMBER):
            json = json_parser_parse_number(parser);
            break;
        case (JSON_TOKEN_TRUE):
            json = json_parser_parse_true(parser);
            break;
        case (JSON_TOKEN_FALSE):
            json = json_parser_parse_false(parser);
            break;
        case (JSON_TOKEN_NULL):
            json = json_parser_parse_null(parser);
            break;
        default:
            fprintf(stderr, "unexpected token\n");
            break;
    }
    if (json == NULL && parser->has_error) {
        fprintf(stderr, "%s\n", parser->error_msg);
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_object(JsonParser *parser) {
    if (!json_parser_expect(parser, JSON_TOKEN_L_BRACE))
        return NULL;
    struct Json *object = (struct Json *) malloc(sizeof(struct Json));
    object->Object.count = 0;
    object->type = JSON_OBJECT;
    object->Object.keys = NULL;
    object->Object.values = NULL;

    // 一直往后面走，直到 JSON_TOKEN_R_BRACE
    //  {"  xxxx ":"",
    while (parser->currentToken.token_type != JSON_TOKEN_R_BRACE) {
        // json_parser_expect(parser, JSON_TOKEN_STRING);
        char *key = strdup(parser->currentToken.value_string);
        if (!json_parser_expect(parser, JSON_TOKEN_STRING))
            return NULL;
        if (!json_parser_expect(parser, JSON_TOKEN_COLON))
            return NULL;
        struct Json *value = json_parser_parse_value(parser);
        object->Object.count++;
        object->Object.keys = realloc(object->Object.keys, sizeof(char *) * object->Object.count);
        object->Object.values = realloc(object->Object.values, sizeof(struct Json *) * object->Object.count);

        object->Object.keys[object->Object.count - 1] = key;
        object->Object.values[object->Object.count - 1] = value;
        /**
         * 如果后续还有节点，则继续往后面走
         */
        if (parser->currentToken.token_type == JSON_TOKEN_COMMA) {
            json_parser_expect(parser, JSON_TOKEN_COMMA);
        }
    }
    if (!json_parser_expect(parser, JSON_TOKEN_R_BRACE))
        return NULL;
    return object;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_array(JsonParser *parser) {
    if (!json_parser_expect(parser, JSON_TOKEN_L_BRACKET))
        return NULL;
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->Array.count = 0;
    json->type = JSON_ARRAY;
    json->Array.elements = NULL;

    while (parser->currentToken.token_type != JSON_TOKEN_R_BRACKET) {
        struct Json *value = json_parser_parse_value(parser);
        if (NULL == value)
            return NULL;
        json->Array.count++;
        json->Array.elements = realloc(json->Array.elements, sizeof(struct Json *) * json->Array.count);
        json->Array.elements[json->Array.count - 1] = value;

        if (parser->currentToken.token_type == JSON_TOKEN_COMMA) {
            json_parser_expect(parser, JSON_TOKEN_COMMA);
        }
    }
    if (!json_parser_expect(parser, JSON_TOKEN_R_BRACKET))
        return NULL;
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_string(JsonParser *parser) {
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->type = JSON_STRING;
    json->value_string = parser->currentToken.value_string;
    if (!json_parser_expect(parser, JSON_TOKEN_STRING)) {
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_number(JsonParser *parser) {
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->type = JSON_NUMBER;
    json->value_number = parser->currentToken.value_number;
    if (!json_parser_expect(parser, JSON_TOKEN_NUMBER)) {
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_true(JsonParser *parser) {
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->type = JSON_TRUE;
    if (!json_parser_expect(parser, JSON_TOKEN_TRUE)) {
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_false(JsonParser *parser) {
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->type = JSON_FALSE;
    if (!json_parser_expect(parser, JSON_TOKEN_FALSE)) {
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(struct Json*) json_parser_parse_null(JsonParser *parser) {
    struct Json *json = (struct Json *) (malloc(sizeof(struct Json)));
    json->type = JSON_NULL;
    if (!json_parser_expect(parser, JSON_TOKEN_NULL)) {
        return NULL;
    }
    return json;
}

CJSON_PUBLIC(int) json_parser_expect(JsonParser *parser, size_t type) {
    if (parser->currentToken.token_type != type) {
        char buf[128];
        snprintf(buf, sizeof(buf), "expected token: %s but got %s\n at %llu,%llu",
                 json_token_to_string(type),
                 json_token_to_string(parser->currentToken.token_type),
                 parser->currentToken.row,
                 parser->currentToken.col
        );
        parser->has_error = 1;
        parser->error_msg = strdup(buf);
        return 0;
    }
    parser->currentToken = json_scanner_next_json_token(parser->scanner);
    return 1;
}

CJSON_PUBLIC(void) json_parser_init(JsonParser *parser, JsonTokenScanner *scanner) {
    parser->scanner = scanner;
    parser->currentToken = json_scanner_next_json_token(scanner);
    parser->has_error = 0;
    parser->error_msg = NULL;
}


//====================== writer ======================//
CJSON_PUBLIC(void) json_writer_init(JsonWriter *writer) {
    writer->capacity = 128;
    writer->length = 0;
    writer->buffer = (char *) malloc(sizeof(char) * writer->capacity);
    writer->buffer[0] = '\0';
}


CJSON_PUBLIC(void) json_writer_expand(JsonWriter *writer, size_t need_extra) {
    while (writer->length + need_extra + 1 > writer->capacity) {
        writer->capacity *= 2;
    }
    writer->buffer = (char *) realloc(writer->buffer, writer->capacity);
}

CJSON_PUBLIC(void) json_writer_append_string(JsonWriter *writer, const char *s) {
    const size_t n = strlen(s);
    json_writer_expand(writer, n);
    memcpy(writer->buffer + writer->length, s, n);
    writer->length += n;
    writer->buffer[writer->length] = '\0';
}

CJSON_PUBLIC(void) json_writer_append_char(JsonWriter *writer, char c) {
    json_writer_expand(writer, 1);
    writer->length += 1;
    writer->buffer[writer->length - 1] = c;
    writer->buffer[writer->length] = '\0';
}


CJSON_PUBLIC(char*) json_writer_release(JsonWriter *writer) {
    return writer->buffer;
}


CJSON_PUBLIC(char*) json_serialize(struct Json *node) {
    JsonWriter writer;
    json_writer_init(&writer);
    json_serialize_value(node, &writer);
    return json_writer_release(&writer);
}

CJSON_PUBLIC(void) json_serialize_value(struct Json *node, JsonWriter *buf) {
    switch (node->type) {
        case JSON_OBJECT:
            json_serialize_object(node, buf);
            break;
        case JSON_ARRAY:
            json_serialize_array(node, buf);
            break;
        case JSON_STRING:
            json_serialize_string(buf, node->value_string);
            break;
        case JSON_NUMBER:
            json_serialize_number(buf, node->value_number);
            break;
        case JSON_TRUE:
            json_writer_append_string(buf, JSON_TRUE_STRING);
            break;
        case JSON_FALSE:
            json_writer_append_string(buf, JSON_FALSE_STRING);
            break;
        case JSON_NULL:
            json_writer_append_string(buf, JSON_NULL_STRING);
            break;
        default:
            break;
    }
}

CJSON_PUBLIC(void) json_serialize_object(struct Json *node, JsonWriter *buf) {
    json_writer_append_char(buf, '{');
    for (int i = 0; i < node->Object.count; i++) {
        if (i > 0)
            json_writer_append_char(buf, ',');
        json_serialize_string(buf, node->Object.keys[i]);
        json_writer_append_char(buf, ':');
        json_serialize_value(node->Object.values[i], buf);
    }
    json_writer_append_char(buf, '}');
}

CJSON_PUBLIC(void) json_serialize_array(struct Json *node, JsonWriter *buf) {
    json_writer_append_char(buf, '[');
    for (int i = 0; i < node->Array.count; i++) {
        if (i > 0)
            json_writer_append_char(buf, ',');
        json_serialize_value(node->Array.elements[i], buf);
    }
    json_writer_append_char(buf, ']');
}

CJSON_PUBLIC(void) json_serialize_string(JsonWriter *writer, const char *s) {
    json_writer_append_char(writer, '"');

    for (; *s; s++) {
        switch (*s) {
            case '"': json_writer_append_string(writer, "\\\"");
                break;
            case '\\': json_writer_append_string(writer, "\\\\");
                break;
            case '/': json_writer_append_string(writer, "\\/");
                break;
            case '\b': json_writer_append_string(writer, "\\b");
                break;
            case '\f': json_writer_append_string(writer, "\\f");
                break;
            case '\n': json_writer_append_string(writer, "\\n");
                break;
            case '\r': json_writer_append_string(writer, "\\r");
                break;
            case '\t': json_writer_append_string(writer, "\\t");
                break;
            default:
                if ((unsigned char) *s < 0x20) {
                    char esc[7];
                    snprintf(esc, sizeof(esc), "\\u%04X", (unsigned char) *s);
                    json_writer_append_string(writer, esc);
                } else {
                    json_writer_append_char(writer, *s);
                }
                break;
        }
    }

    json_writer_append_char(writer, '"');
}

CJSON_PUBLIC(void) json_serialize_number(JsonWriter *writer, double value_double) {
    char tmp[64];
    int n = snprintf(tmp, sizeof(tmp), "%g", value_double);
    json_writer_append_string(writer, tmp);
    (void) n;
}
