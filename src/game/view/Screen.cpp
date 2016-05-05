#include "Screen.h"

Screen::Screen( SDL_Renderer* renderer ) {
  this->renderer = renderer;
  this->textures = NULL;
  this->figures = NULL;
}

Screen::~Screen() {
  if( this->textures != NULL ) {

  }

  if( this->figures != NULL ) {

  }
}

void Screen::loadTexture( string id, string path ) {
  Texture* t = new Texture( this->renderer );
  if( t->loadFromFile( path ) ) {
    // Create hash if it doesn't exist
    if( this->textures == NULL )
      this->textures = new map<string, Texture*>();
    // Add texture to hash
    this->textures->insert( pair<string, Texture*>(id, t) );
  }
}
