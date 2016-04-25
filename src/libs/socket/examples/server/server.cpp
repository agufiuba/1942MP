#include "../../Server.h"

int main(int argc, char* argv[]) {
  const char* configFileName = argv[1] ? argv[1] : "default-sc.xml"; 

  Server* server = new Server( configFileName );

  delete server;

  return 0;
}
