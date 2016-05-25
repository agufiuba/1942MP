#include "HealthView.h"
#include <string>
using namespace std;

HealthView::HealthView( Screen* screen, int heartCount ) {
 this->screen = screen;
 this->heartCount = heartCount;
 this->loadView();
}

HealthView::~HealthView() {}

void HealthView::loadView() {
  string id = "heart";
  string path = "heart.bmp";
  this->screen->loadTexture( id, path );
}

void HealthView::render() {
  string id = "heart";
  int posX = 0, posY = 10;
  int spacing = 10;
  int imageWidth = this->screen->getTextureWidth( id );
  for ( int i = 0; i < this->heartCount; i++ ) {
    this->screen->renderTexture( id, posX, posY );
    posX += imageWidth + spacing;
  }
}

void HealthView::update( int heartCount ) {
  this->heartCount = heartCount;
}
