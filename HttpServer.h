#ifndef HttpServer_h
#define HttpServer_h

#include "Server.h"
#include "HttpRequest.h"

struct HttpServer {
    struct Server server;
    struct Dictionary routes;
    void (*register_routes) (struct HttpServer *server, void (*route_function) (struct HttpServer *server, struct HttpRequest *request), 
    char *uri, int num_methods, ...);
};

struct HttpServer http_server_constructor(void); 

#endif