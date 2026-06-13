//
// Created by echo on 2026/6/13.
//

#ifndef CJSON_FORK_CJSON_H
#define CJSON_FORK_CJSON_H

#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif


#if !defined(__WINDOWS__) && ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_MSC_VER))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

#define CJSON_CDECL   __cdecl
#define CJSON_STDCALL __stdcall

#if !defined(CJSON_HIDE_SYMBOLS) &&  !defined(CJSON_IMPORT_SYMBOLS) && !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type) type CJSON_STDCALL
#endif

#if defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type) __declspec(dllexport) type CJSON_STDCALL

#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type) __declspec(dllimport) type CJSON_STDCALL
#define
#endif

#else
#define CJSON_CDECL
#define CJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif

#endif

#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 0
#define CJSON_VERSION_PATCH 0

#define STR_X(x)  #x
#define STR(x)    STR_X(x)

#define CJSON_VERSION  \
   STR(CJSON_VERSION_MAJOR) "." \
   STR(CJSON_VERSION_MINOR) "." \
   STR(CJSON_VERSION_PATCH)

#define  JSON_TRUE_STRING  "true"
#define  JSON_FALSE_STRING "false"
#define  JSON_NULL_STRING  "null"
#define  JSON_BUFFER_EXPAND_FACTOR 2



//====================== lexer ======================//
typedef enum {
    JSON_TOKEN_L_BRACE, //{
    JSON_TOKEN_R_BRACE,

    JSON_TOKEN_L_BRACKET, //[
    JSON_TOKEN_R_BRACKET,

    JSON_TOKEN_COMMA,  //,
    JSON_TOKEN_COLON,  //:

    JSON_TOKEN_NUMBER,
    JSON_TOKEN_STRING,

    JSON_TOKEN_FALSE,
    JSON_TOKEN_TRUE,
    JSON_TOKEN_NULL,

    JSON_TOKEN_ERROR,
    JSON_TOKEN_EOF,
}JsonTokenType;

struct JsonToken {
    JsonTokenType token_type;
    size_t        col;
    size_t        row;

    const  char*  start;
    int           length;

    const  char*  value_string;
    double        value_number;

    const  char*  error_msg;
};
CJSON_PUBLIC(void) json_token_print(struct JsonToken *token);
CJSON_PUBLIC(const char *)json_token_to_string(const JsonTokenType token_type);


typedef struct JsonTokenScanner {
    const  char*  start;
    const  char*  current;
    int           col;
    int           row;
}JsonTokenScanner;

CJSON_PUBLIC(char) json_scanner_advance(JsonTokenScanner *scanner);
CJSON_PUBLIC(char) json_scanner_peek(JsonTokenScanner *scanner);
CJSON_PUBLIC(void) json_scanner_skip_whitespace(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_new_json_token(JsonTokenScanner *scanner, JsonTokenType token_type);
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_number(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_string(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_true(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_false(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_try_parse_null(JsonTokenScanner *scanner);
CJSON_PUBLIC(struct JsonToken) json_scanner_next_json_token(JsonTokenScanner *scanner);
CJSON_PUBLIC(void) json_scanner_init_scanner(JsonTokenScanner *scanner,const char* json);



//====================== parser ======================//
#define JSON_INVALID  (0)
#define JSON_OBJECT   (1 << 0)
#define JSON_ARRAY    (1 << 1)
#define JSON_STRING   (1 << 2)
#define JSON_NUMBER   (1 << 3)
#define JSON_TRUE     (1 << 4)
#define JSON_FALSE    (1 << 5)
#define JSON_NULL     (1 << 6)
#define JSON_RAW      (1 << 7)

struct Json {
    size_t type;

    union {
        double value_number;
        const char *value_string;

        struct Object {
            char **keys;
            struct Json **values;
            int count;
        } Object;

        struct Array {
            struct Json **elements;
            int count;
        } Array;
    };
};

typedef struct JsonParser {
    JsonTokenScanner *scanner;
    struct JsonToken currentToken;
    int    has_error;
    const char* error_msg;
}JsonParser;

/**
 *    JSON    ::= Value
 *    Value   ::= Object | Array | String | Number | true | false | null
 *    Object  ::= '{' MemberList? '}'
 *    MemberList ::= Member (',' Member)*
 *    Member ::= String ':' Value
 *    Array   ::= '[' ValueList? ']'
 *    ValueList ::= Value (',' Value)*
 */
CJSON_PUBLIC(struct Json*) json_parser_parse(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_value(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_object(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_array(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_string(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_number(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_true(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_false(JsonParser* parser);
CJSON_PUBLIC(struct Json*) json_parser_parse_null(JsonParser* parser);

CJSON_PUBLIC(int)  json_parser_expect(JsonParser* parser,size_t type);
CJSON_PUBLIC(void) json_parser_init(JsonParser* parser,JsonTokenScanner* scanner);



//====================== writer ======================//
typedef struct JsonWriter {
    char*    buffer;
    size_t   length;
    size_t   capacity;
}JsonWriter;

CJSON_PUBLIC(void) json_writer_init(JsonWriter *writer);
CJSON_PUBLIC(void) json_writer_expand(JsonWriter *writer, size_t need_extra);
CJSON_PUBLIC(void) json_writer_append_string(JsonWriter *writer, const char *s);
CJSON_PUBLIC(void) json_writer_append_char(JsonWriter *writer, char c);
CJSON_PUBLIC(char) *json_writer_release(JsonWriter *writer);
CJSON_PUBLIC(void) json_serialize_value(struct Json *node, JsonWriter *buf);
CJSON_PUBLIC(void) json_serialize_object(struct Json *node, JsonWriter *buf);
CJSON_PUBLIC(void) json_serialize_array(struct Json *node, JsonWriter *buf);
CJSON_PUBLIC(void) json_serialize_string(JsonWriter *buf, const char *string);
CJSON_PUBLIC(void) json_serialize_number(JsonWriter *buf, double value_double);

CJSON_PUBLIC(char) *json_serialize(struct Json *node);
#ifdef __cplusplus
}
#endif











#endif //CJSON_FORK_CJSON_H
