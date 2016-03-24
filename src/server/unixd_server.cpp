#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

using namespace std;

int main() {
  
  typedef struct sockaddr_un {
    sa_family_t sun_family; /* Always AF_UNIX */
    char sun_path[108];	    /* Null-terminated socket pathname */
  } sockaddr_un;

  const char* SOCKNAME = "/tmp/5dsock";
  int sfd, cfd;
  sockaddr_un addr;
  const int BACKLOG = 5;
  ssize_t numRead;
  const int BUF_SIZE = 100;
  char buf[BUF_SIZE];

  // Create socket
  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(sfd == -1) {
    cout << "socket error" << endl;
    exit(0);
  }

  // Remove socket bind if exists
  if(remove(SOCKNAME) == -1 && errno != ENOENT) {
    cout << "remove error" << endl;
    exit(0);
  }

  // Clear structure
  memset(&addr, 0, sizeof(sockaddr_un));

  // Domain address
  addr.sun_family = AF_UNIX;  
  strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);

  // Bind socket
  if(bind(sfd, (sockaddr*) &addr, sizeof(sockaddr_un)) == -1) {
    cout << "bind error" << endl;
    exit(0);
  }

  // Listen for connections
  if(listen(sfd, BACKLOG) == -1) {
    cout << "listen error" << endl;
    exit(0);
  }

  // Handle client connections
  for(;;) {
    cfd = accept(sfd, NULL, NULL);
    if(cfd == -1) {
      cout << "accept error" << endl;
      exit(0);
    }

    // Transfer data from connected socket to stdout until EOF
    while((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
      if(write(STDOUT_FILENO, buf, numRead) != numRead) {
	cout << "partial/failed write" << endl;
        exit(0);
      }
    }

    if(numRead == -1) {
      cout << "read error" << endl;
      exit(0);
    }

    if(close(cfd) == -1) {
      cout << "close error" << endl;
      exit(0);
    }
  }

  return 0;
}
