// server.cpp
#include <sys/socket.h>
#include <netdb.h> /* addrinfo */
#include <netinet/in.h>
#include <unistd.h> /* close() */
#include <arpa/inet.h> /* inet_ntop() */
#include <cstdint> /* uint32_t */
#include <csignal> /* signal(,) */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(,,) */
#include <iostream>

using namespace std;

int main() {

  uint32_t seqNum = 0;
  int sfd, cfd;
  socklen_t sinSize;
  struct sockaddr_in server, client;
  const int PORT = 5340;
  const int BACKLOG = 5;

  /* Prevent the server from receiving the SIGPIPE signal
     instead the write() fails with the error EPIPE */
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    cout << "signal error" << endl;
    exit(EXIT_FAILURE);    
  }

  /* Create TCP socket */
  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cout << "socket error" << endl;
    exit(-1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  
  bzero(&(server.sin_zero), 8);

  /* Bind socket */
  if(bind(sfd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
    cout << "bind error" << endl;
    exit(-1);
  }

  /* Listen for connections */
  if(listen(sfd, BACKLOG) == -1) {
    cout << "listen error" << endl;
    exit(-1);
  }

  /* Accept connections */
  for(;;) {
   sinSize = sizeof(struct sockaddr);
   if((cfd = accept(sfd, (struct sockaddr*) &client, &sinSize)) == -1) {
    cout << "accept error" << endl;
    exit(-1);
   }

   cout << "Se inicio una conexion con el cliente: " << inet_ntoa(client.sin_addr) << endl;
   send(cfd, "Bienvenido a mi servidor.\n", 25, 0);

   close(cfd);
  }

  return 0;
}
