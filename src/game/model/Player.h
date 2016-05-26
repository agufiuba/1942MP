#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Player {
  private:
    string name;
    string color;
    int x;
    int y;
    bool active;
    int health;
    int score;
    int team;

  public:
    Player( string name, string color, int x, int y, int team, int health = 3 );
    ~Player();
    string getName();
    string getColor();
    int getScore();
    void addScore( int score );
    int getX();
    int getY();
    int getTeam();

    void setX(int x);
    void setY(int y);

    bool isActive();
    void activate();
    void deactivate();
    int getHealth();
    void takeHit();
    bool isAlive();
};
#endif
