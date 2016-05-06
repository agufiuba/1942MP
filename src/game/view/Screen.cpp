#include "Screen.h"
#include <iostream>

Screen::Screen( XM_SDL* sdlHandler ) {
  this->sdlHandler = sdlHandler;
  this->renderer = this->sdlHandler->getRenderer();
  this->loadFont( "fonts/open_sans/OpenSans-Regular.ttf" );
  this->fontColor = { 0, 0, 0, 255 };
}

Screen::~Screen() {
  // Delete textures
  if( !( this->textures.empty() ) ) {
    for( map<string, Texture*>::iterator it = this->textures.begin();
	 it != this->textures.end();
	 ++it ) {
      delete it->second;
    }
  }

  // Delete figures
  if( !( this->figures.empty() ) ) {

  }
}

void Screen::loadFont( const char* FONT_PATH ) {
  TTF_Font* loadedFont = this->sdlHandler->loadFont( FONT_PATH );
  if( loadedFont != NULL ) {
    this->fontFamily = loadedFont;
    loadedFont = NULL;
  }
}

void Screen::addTexture( string id, Texture* t ) {
  // Add texture to hash
  this->textures[ id ] = t;
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

void Screen::renderTexture( string id, int x, int y ) {
  map<string, Texture*>::iterator it = this->textures.find( id );
  if( it == this->textures.end() ) {
    cout << endl << id << " texture not found!" << endl;
    return;
  }
  
  it->second->render( x, y );
}

int Screen::getTextureWidth( string id ) {
  map<string, Texture*>::iterator it = this->textures.find( id );
  if( it == this->textures.end() ) {
    cout << endl << id << " texture not found!" << endl;
    return -1;
  }
  return it->second->getWidth();
}
