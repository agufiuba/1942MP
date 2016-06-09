#ifndef SCORE_VIEW_H
#define SCORE_VIEW_H
#include "Screen.h"
#include <string>
using namespace std;

class ScoreView {
  private:
    Screen* screen;
    string displayText;
    int score;
    void loadView();

  public:
    ScoreView( Screen* screen, int score = 0, string displayText = "Score" );
    ~ScoreView();
    void render( string positionFlag = "C" );
    void update( int score );
};
#endif
