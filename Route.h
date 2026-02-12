#ifndef Route_h
#define Route_h

#include "HttpRequest.h"
#include "HttpServer.h"

struct Route {
    int methods[9];
    char *uri;
    char * (*route_func)(struct HttpServer *server, struct HttpRequest *request);
};

#endif