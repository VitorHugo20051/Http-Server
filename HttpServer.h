#ifndef HttpServer_h
#define HttpServer_h

#include "Server.h"
#include "HttpRequest.h"
#include "ThreadPool.h"

struct HttpServer {
    struct Server server;
    struct Dictionary routes;
    void (*register_routes) (struct HttpServer *server, char * (*route_function) (struct HttpServer *server, struct HttpRequest *request), 
    char *uri, int num_methods, ...);
    void (*launch)(struct HttpServer *server);
};

struct HttpServer http_server_constructor(void); 

char * render_template(int num_templates, ...);

#endif