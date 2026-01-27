#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "HttpRequest.h"

void launch(struct Server *server) {
    int addr_len = sizeof(server->address);
    long valread;

    while(1) {
        printf("=== WAITING ===\n");

        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&addr_len);
        char buffer[30000];
        valread = read(new_socket, buffer, 30000);

        struct HttpRequest request = http_request_constructor(buffer);

        retrive_page(request, new_socket);
        close(new_socket);
    }
}

void retrive_page(struct HttpRequest request, int socket) {
    char path[30000] = {0};

    char *url = strtok(request.request_line.search(&request.request_line, "uri", sizeof("uri")), "?");
}

int main() {
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    server.launch(&server);
    return 0;
}