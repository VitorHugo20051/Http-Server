#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

void launch(struct Server *server) {
    char buffer[30000];
    char path[256];
    int address_lenght = sizeof(server->address);
    int new_socket;

    while(1) {
        printf("===== WAITING FOR CONNECTION =====\n");

        new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_lenght);
        read(new_socket, buffer, 30000);

        printf("%s\n", buffer);

        
        close(new_socket);
    }
}

int main() {
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    server.launch(&server);
    return 0;
}