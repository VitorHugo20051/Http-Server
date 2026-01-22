#include "HttpRequest.h"
#include <string.h>

int method_select(char *method) {
    switch (method)
    {
        case "GET":
            return GET;
        
        case "POST":
            return POST;
        
        case "PUT":
            return PUT;

        case "HEAD":
            return HEAD;
    
        case "PATCH":
            return PATCH;

        case "DELETE":
            return DELETE;

        case "CONNECT":
            return CONNECT;

        case "OPTIONS":
            return OPTIONS;

        case "TRACE":
            return TRACE;
    }
}    

struct HttpRequest http_request_constructor(char *request_string) {
    for(int i = 0; i < strlen(request_string) - 1; i++) {
        if (request_string[i] == '\n' && request_string[i + 1] == '\n') {
            request_string[i + 1] = '|';
        }
    }
    
    char *request_line = strtok(request_string, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    struct HttpRequest request;
    
    char *method = strtok(request_line, " ");

    request.Method = method_select(method);

    return request;
};
