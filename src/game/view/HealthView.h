#ifndef HEALTH_VIEW_H
#define HEALTH_VIEW_H
#include "Screen.h"
class HealthView {
  private:
    Screen* screen;
    int heartCount;
    void loadView();

  public:
    HealthView( Screen* screen, int heartCount = 4 );
    ~HealthView();
    void render();
    void update( int heartCount );
};
#endif
