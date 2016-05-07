#include "../../../Client.h"
#include <string>
using namespace std;

int main() {
  string name = "sosuke";
  string color = "blue";
  // Fill player data struct with data
  PlayerData* data = new PlayerData;
  strcpy( data->name, name.c_str() );
  strcpy( data->color, color.c_str() );

  Client* client = new Client( "127.0.0.1", "8000" );
  client->connectToServer();
  client->sendData( data );

  delete client;
  delete data;
  return 0;
}
