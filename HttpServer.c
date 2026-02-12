#include "HttpServer.h"
#include "Route.h"
#include "ThreadPool.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void register_routes (struct HttpServer *server, char * (*route_function) (struct HttpServer *server, struct HttpRequest *request), 
    char *uri, int num_methods, ...);

void launch(struct HttpServer *server);

void * handler(void *arg);

char *render_template(int num_templates, ...);

struct ClientServer {
    int client;
    struct HttpServer *server;
};

struct HttpServer http_server_constructor() {
    struct HttpServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, INADDR_ANY, 0, 80, 255);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes;
    server.launch = launch;
    return server;
}

void register_routes (struct HttpServer *server, char * (*route_function) (struct HttpServer *server, struct HttpRequest *request), char *uri, int num_methods, ...) {
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);
    for (int i = 0; i < num_methods; i++)
    {
        route.methods[i] = va_arg(methods, int);
    }
    char buffer[strlen(uri)];
    route.uri = buffer;
    strcpy(route.uri, uri);
    route.route_func = route_function;
    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HttpServer *server) {
    struct ThreadPool *thread_pool = thread_pool_constructor(20);
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    socklen_t address_length = sizeof(server->server.address);
    while(1) {
        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        client_server->client = accept(server->server.socket, sock_addr, &address_length);
        client_server->server = server;
        struct ThreadJob job = thread_job_constructor(handler, client_server);
        thread_pool->add_work(thread_pool, job);
    }
}

void * handler(void *arg) {
    struct ClientServer *client_server = (struct ClientServer *)arg;
    
    char request_string[30000] = {0};
    read(client_server->client, request_string, 30000);

    struct HttpRequest request = http_request_constructor(request_string);
    
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    
    struct Route *route = NULL;
    if (uri) {
        route = client_server->server->routes.search(&client_server->server->routes, uri, strlen(uri) + 1);
    }

    if (route && route->route_func) {
        char *body = route->route_func(client_server->server, &request);
        if (body) {
            char header[512];
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n", strlen(body));
            write(client_server->client, header, strlen(header));
            write(client_server->client, body, strlen(body));
            free(body); 
        }
    } else {
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        write(client_server->client, not_found, strlen(not_found));
    }

    close(client_server->client);
    
    free(client_server);
    return NULL;
}

char * render_template(int num_templates, ...) {
    char *buffer = malloc(30000);
    memset(buffer, 0, 30000);
    int buffer_position = 0;
    va_list files;
    va_start(files, num_templates);

    for (int i = 0; i < num_templates; i++) {
        char *path = va_arg(files, char*);
        FILE *file = fopen(path, "r");
        
        if (file == NULL) {
            printf("ERRO DE LEITURA: Não foi possível abrir o ficheiro em [%s]\n", path);
            continue;
        }

        char c;
        while ((c = fgetc(file)) != EOF && buffer_position < 29999) {
            buffer[buffer_position++] = c;
        }
        fclose(file);
    }
    va_end(files);
    buffer[buffer_position] = '\0';
    
    return buffer;
}