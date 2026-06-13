#include <stdio.h>
#include "Cjson.h"

int main(void) {
    printf("%s\n",CJSON_VERSION);

    const char *json = "{\"name\":\"JSON Parser\","
                    "\"version\":1.120,\"active\":true,"
                    "\"tags\":[\"json\",\"parser\",123],\n\t\f\v\n\n\n \"config\":"
                    "{\"debug\":false,\"max\":null,\"ratio"
                    "\":-3.14e-2}}";
    printf("%s\n",json);

    JsonTokenScanner scanner;
    json_scanner_init_scanner(&scanner,json);

    int token_count = 0;
    while (1) {
        struct JsonToken token = json_scanner_next_json_token(&scanner);
        json_token_print(&token);
        token_count++;

        if (token.token_type == JSON_TOKEN_EOF || token.token_type == JSON_TOKEN_ERROR) {
            break;
        }
    }
    printf("\n=== Total tokens: %d ===\n", token_count);


    return 0;
}
