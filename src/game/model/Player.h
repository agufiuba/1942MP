#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Player {
  private:
    string name;
    string color;
    bool active;

  public:
    Player( string name, string color );
    ~Player();
    string getName();
    string getColor();
    bool isActive();
    void deactivate();
};
#endif

