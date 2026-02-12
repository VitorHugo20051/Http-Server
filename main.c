#include "HttpServer.h"

char * home(struct HttpServer *server, struct HttpRequest *request) {
    (void)server; 
    (void)request;
    
    return render_template(1, "/home/vitordev/response/index.html");
}

int main() {
  struct HttpServer server = http_server_constructor();

  server.register_routes(&server, home, "/", 0);

  server.launch(&server);
}