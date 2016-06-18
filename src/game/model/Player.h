#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
using namespace std;
class Player {
  private:
    string name;
    string color;
    int x;
    int y;
    int xEstacionamiento;
    int yEstacionamiento;
    bool active;
    int health;
    int score;
    int team;
    bool ready;

  public:
    Player( string name, string color, int x, int y, int team, int health = 3 );
    Player( string name, string color, int x, int y, int xEst, int yEst, int team, int health = 3 );
    ~Player();
    string getName();
    string getColor();
    int getScore();
    void addScore( int score );
    void resetScore();
    int getX();
    int getY();
    int getEstacionamientoX();
    int getEstacionamientoY();
    int getTeam();

    void setX(int x);
    void setY(int y);

    bool isActive();
    void activate();
    void deactivate();
    int getHealth();
    void takeHit();
    bool isAlive();
    void die();
    void changeReady();
    bool isReady();
    void setTeam(int team);
};
#endif
