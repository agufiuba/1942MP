#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Player {
  private:
    string name;
    string color;

  public:
    Player( string name, string color );
    ~Player();
    string getName();
    string getColor();
};
#endif

