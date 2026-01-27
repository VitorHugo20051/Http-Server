#ifndef HttpRequest_h
#define HttpRequest_h

#include "DataStructures/Dictionary/Dictionary.h"

enum HttpMethods {
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
};

struct HttpRequest {
    int Method;
    char *URI;
    float HttpVersion;
    struct Dictionary header_fields;
};

struct HttpRequest http_request_constructor(char *request_string);


#endif