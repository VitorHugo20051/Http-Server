#include "HttpServer.h"
#include "Route.h"
#include "ThreadPool.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void register_routes (struct HttpServer *server, void (*route_function) (struct HttpServer *server, struct HttpRequest *request), 
    char *uri, int num_methods, ...);

void launch(struct HttpServer *server);

void * handler(void *arg);

struct ClientServer {
    int client;
    struct HttpServer *server;
};

struct HttpServer http_server_constructor() {
    struct HttpServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, INADDR_ANY, 0, 80, 255);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes;
    return server;
}

void register_routes (struct HttpServer *server, void (*route_function) (struct HttpServer *server, struct HttpRequest *request), char *uri, int num_methods, ...) {
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);

    for (int i = 0; i < num_methods; i++) {
        route.methods[i] = va_arg(methods, int);
    }

    strcpy(route.uri, uri);
    route.route_func = route_function;
    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HttpServer *server) {
    struct ThreadPool thread_pool = thread_pool_constructor(20);
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    unsigned long address_length = sizeof(server->server.address);
    while(1) {
        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        client_server->client = accept(server->server.socket, sock_addr, address_length);
        client_server->server = server;
        struct ThreadJob job = thread_job_constructor(handler, client_server);
        thread_pool.add_work(&thread_pool, job);
    }
}

void * handler(void *arg) {
    struct ClientServer *client_server = (struct ClientServer *)arg;
    free(arg);

    char *request_string[30000];
    read(client_server->client, request_string, 30000);
    struct HttpRequest request = http_request_constructor(request_string);

    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)])); 

    char *response = route->route_func(client_server->server, &request);
    write(client_server->client, response, sizeof(char[strlen(response)]));
    close(client_server->client);

    free(client_server);

    http_request_destructor(&request);

    return NULL;
}