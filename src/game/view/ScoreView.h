#ifndef SCORE_VIEW_H
#define SCORE_VIEW_H
#include "Screen.h"
class ScoreView {
  private:
    Screen* screen;
    int score;
    void loadView();

  public:
    ScoreView( Screen* screen, int score = 0 );
    ~ScoreView();
    void render();
    void update( int score );
};
#endif
