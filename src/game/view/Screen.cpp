#include "Screen.h"
#include <iostream>

Screen::Screen( XM_SDL* sdlHandler ) {
  this->sdlHandler = sdlHandler;
  this->renderer = this->sdlHandler->getRenderer();
  this->loadFont( "src/initialWindow/fonts/open_sans/OpenSans-Regular.ttf" );
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

void Screen::loadRectangle( string id, int x, int y, int width, int height ) {
  SDL_Rect r = { x, y, width, height };
  this->rectangles[ id ] = r;
}

void Screen::loadText( string id, string value, SDL_Color textColor ) {
  Texture* t = new Texture( this->renderer );
  if( t->loadFromRenderedText( value, this->fontFamily, textColor ) ) {
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

void Screen::renderRectangle( string id, bool outline ) {
  map<string, SDL_Rect>::iterator it = this->rectangles.find( id );
  if( it == this->rectangles.end() ) {
    cout << endl << id << " rectangle not found!" << endl;
    return;
  }

  if( outline ) {
    SDL_RenderDrawRect( this->renderer, &it->second );
  } else {
    SDL_RenderFillRect( this->renderer, &it->second );
  }
}

int Screen::getTextureWidth( string id ) {
  map<string, Texture*>::iterator it = this->textures.find( id );
  if( it == this->textures.end() ) {
    cout << endl << id << " texture not found!" << endl;
    return -1;
  }
  return it->second->getWidth();
}

int Screen::getTextWidth( string text ) {
  int width;
  TTF_SizeText( this->fontFamily, text.c_str(), &width, NULL );
  return width;
}

void Screen::setRenderDrawColor( int r, int g, int b, int a ) {
  SDL_SetRenderDrawColor( this->renderer, r, g, b, a );
}
