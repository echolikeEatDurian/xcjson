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
        token.error_msg = buf;
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
            token.error_msg = buf;
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
            token.error_msg = buf;
            return token;
        }
        while (isdigit(json_scanner_peek(scanner))) {
            json_scanner_advance(scanner);
        }
    }
    return json_scanner_new_json_token(scanner, JSON_TOKEN_NUMBER);
}

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
    errorToken.error_msg = buf;
    return errorToken;
}

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
    errorToken.error_msg = buf;
    return errorToken;
}

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
    token.error_msg = buf;
    return token;
}

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
    token.error_msg = buf;
    return token;
}

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
            token.error_msg = buf;
    }
    return token;
}

CJSON_PUBLIC(void) json_scanner_init_scanner(JsonTokenScanner *scanner, const char *json) {
    scanner->row = 1;
    scanner->col = 1;
    scanner->start = json;
    scanner->current = json;
}

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

    printf("[Line %-3d, Col %-3d] %-12s: ", token->row, token->col, type_names[token->token_type]);

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
