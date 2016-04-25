#include "../Client.h"

int main( int argc, char* argv[] ) {
  const char* configFileName = argv[1] ? argv[1] : "default-cc.xml";

  Client* client = new Client( configFileName );

  delete client;

  return 0;
}
