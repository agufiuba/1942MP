#include <sys/socket.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

int main() {
  
  typedef struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[108];
  } sockaddr_un;

  sockaddr_un addr;
  int sfd;
  const int BUF_SIZE = 100;
  ssize_t numRead;
  char buf[BUF_SIZE];
  const char* SV_SOCK_PATH = "/tmp/5dsock";

  // Create socket
  sfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if(sfd == -1) {
    cout << "socket error" << endl;
    exit(0);
  }

  // Construct server address, and make the connection
  memset(&addr, 0, sizeof(sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if(connect(sfd, (sockaddr*) &addr, sizeof(sockaddr_un)) == -1) {
    cout << "connect error" << endl;
    exit(0);
  }

  // Copy stdin to socket
  while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if(write(sfd, buf, numRead) != numRead) {
      cout << "partial/failed write" << endl;
      exit(0);
    }

    if(numRead == -1) {
      cout << "read error" << endl;
      exit(0);
    }

    // Close socket, server sees EOF
    exit(EXIT_SUCCESS);
  }

  return 0;
}
