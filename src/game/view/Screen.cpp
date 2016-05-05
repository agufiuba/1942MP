#include "Screen.h"

Screen::Screen( XM_SDL* sdlHandler ) {
  this->sdlHandler = sdlHandler;
  this->renderer = this->sdlHandler->getRenderer();
  this->textures = NULL;
  this->figures = NULL;
  this->loadFont( "fonts/open_sans/OpenSans-Regular.ttf" );
  this->fontColor = { 0, 0, 0, 255 };
}

Screen::~Screen() {
  // Delete textures
  if( this->textures != NULL ) {
    for( map<string, Texture*>::iterator it = this->textures->begin();
	 it != this->textures->end();
	 ++it ) {
      delete it->second;
    }
  }

  // Delete figures
  if( this->figures != NULL ) {

  }
}

void Screen::loadFont( const char* FONT_PATH ) {
  TTF_Font* loadedFont = this->sdlHandler->loadFont( FONT_PATH );
  if( loadedFont != NULL ) {
    this->fontFamily = loadedFont;
    loadedFont = NULL;
  }
}

void Screen::initTextures() {
  // Create hash if it doesn't exist
  if( this->textures == NULL )
    this->textures = new map<string, Texture*>();
}

void Screen::addTexture( string id, Texture* t ) {
  this->initTextures();
  // Add texture to hash
  this->textures->insert( pair<string, Texture*>(id, t) );
}

void Screen::loadTexture( string id, string path ) {
  Texture* t = new Texture( this->renderer );
  if( t->loadFromFile( path ) ) {
    this->addTexture( id, t );
  }
}

void Screen::loadText( string id, string value ) {
  Texture* t = new Texture( this->renderer );
  if( t->loadFromRenderedText( value, this->fontFamily, this->fontColor ) ) {
    this->addTexture( id, t );
  }
}
