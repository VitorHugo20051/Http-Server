#ifndef HttpRequest_h
#define HttpRequest_h

#include "DataStructures/Dictionary/Dictionary.h"

struct HttpRequest {
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary body;
};

struct HttpRequest http_request_constructor(char *request_string);
void http_request_destructor(struct HttpRequest *request);

#endif