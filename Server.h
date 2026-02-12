#ifndef Server_h
#define Server_h

#include <netinet/in.h>
#include <sys/socket.h>

struct Server {
  int domain;
  int service;
  int protocol;
  u_long interface;
  int port;
  int backlog;

  struct sockaddr_in address;

  int socket;
};

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog);

#endif