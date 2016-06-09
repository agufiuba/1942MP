#include "ScoreView.h"

ScoreView::ScoreView( Screen* screen, int score, string displayText ) {
  this->screen = screen;
  this->score = score;
  this->displayText = displayText;
  this->loadView();
}

ScoreView::~ScoreView() {}

void ScoreView::loadView() {
  this->screen->loadText( this->displayText, this->displayText, { 226, 229, 38, 255 } );
  this->screen->loadText( this->displayText + "score", to_string( this->score ), { 255, 255, 255, 255 } );
}

void ScoreView::render( string positionFlag ) {
  int displayTextCenter;
  int padding = 20;
  // Center
  if ( positionFlag == "C" ) 
    displayTextCenter = this->screen->getTextCenter( this->displayText );

  // Right margin
  if ( positionFlag == "R" )
    displayTextCenter = this->screen->getCanvasWidth() - ( this->screen->getTextWidth( this->displayText ) + padding );

  int posY = 0;
  int scorePosY = this->screen->getTextHeight( this->displayText );
  string scoreR = to_string( this->score );
  int scoreCenter = displayTextCenter + this->screen->getTextWidth( this->displayText ) - this->screen->getTextWidth( scoreR ); 
  this->screen->renderTexture( this->displayText, displayTextCenter, posY );
  this->screen->renderTexture( this->displayText + "score", scoreCenter, scorePosY );
}

void ScoreView::update( int score ) {
  this->score = score;
  this->screen->loadText( this->displayText + "score", to_string( this->score ), { 255, 255, 255, 255 } );
}
