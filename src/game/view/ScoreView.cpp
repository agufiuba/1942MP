#include "ScoreView.h"
#include <string>
using namespace std;

ScoreView::ScoreView( Screen* screen, int score ) {
  this->screen = screen;
  this->score = score;
  this->loadView();
}

ScoreView::~ScoreView() {}

void ScoreView::loadView() {
  this->screen->loadText( "scoreText", "score", { 226, 229, 38, 255 } );
  this->screen->loadText( "score", to_string( this->score ), { 255, 255, 255, 255 } );
}

void ScoreView::render() {
  int posY = 0;
  int scorePosY = this->screen->getTextHeight( "score" );
  string scoreR = to_string( this->score );
  int scoreCenter = this->screen->getTextCenter( "score" ) + this->screen->getTextWidth( "score" ) - this->screen->getTextWidth( scoreR ); 
  this->screen->renderTexture( "scoreText", this->screen->getTextCenter( "score" ), posY );
  this->screen->renderTexture( "score", scoreCenter, scorePosY );
}

void ScoreView::update( int score ) {
  this->score = score;
  this->screen->loadText( "score", to_string( this->score ), { 255, 255, 255, 255 } );
}
