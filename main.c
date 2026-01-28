#include "HttpRequest.h"
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void retrive_page(struct HttpRequest request, int socket);

void launch(struct Server *server) {
  int addr_len = sizeof(server->address);
  long valread;

  while (1) {
    printf("=== WAITING ===\n");

    int new_socket = accept(server->socket, (struct sockaddr *)&server->address,
                            (socklen_t *)&addr_len);
    char buffer[30000];
    valread = read(new_socket, buffer, 30000);
    printf("%s", buffer);
    struct HttpRequest request = http_request_constructor(buffer);

    retrive_page(request, new_socket);
    close(new_socket);
  }
}

void retrive_page(struct HttpRequest request, int socket) {
  char path[30000] = {0};

  char *base_path = "/home/vitordev/response";
  char *url = strtok(
      request.request_line.search(&request.request_line, "uri", sizeof("uri")),
      "?");

  if (url == NULL || strcmp(url, "/") == 0) {
    sprintf(path, "%s/index.html", base_path);
  } else {
    sprintf(path, "%s%s", base_path, url);
  }

  FILE *f = fopen(path, "r");
  if (f == NULL) {
    perror("Error opening");
    write(socket, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
    return;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = malloc(fsize + 1);
  fread(buffer, 1, fsize, f);
  fclose(f);

  char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  write(socket, header, strlen(header));
  write(socket, buffer, fsize);
}

int main() {
  struct Server server =
      server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 20, launch);
  server.launch(&server);
}
