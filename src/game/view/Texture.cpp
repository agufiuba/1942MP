#include "Texture.h"
#include <iostream>
using namespace std;

Texture::Texture( SDL_Renderer* renderer )
{
  //Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  this->renderer = renderer;
}

Texture::~Texture()
{
  //Deallocate
  free();
}

bool Texture::loadFromFile( std::string path)
{
  //Get rid of preexisting texture
  free();

  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* image = NULL;

  string direccion = DIR_IMAGE+path;
  image = SDL_LoadBMP( direccion.c_str() );
  if( image == NULL ) {
	  path = DESCONOCIDO;
	  image = SDL_LoadBMP( path.c_str() );
  }
  if( image == NULL ) {
	cout << "Unable to load image.\nSDL error: " << SDL_GetError() << endl;
  } else {
	//Color de la imagen jugador (le saco el fondo)
	SDL_SetColorKey( image, SDL_TRUE, SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) );

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface( renderer, image );
	if( newTexture == NULL ){
	  printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	} else {
	  //Get image dimensions
	  mWidth = image->w;
	  mHeight = image->h;
	}
  }

  SDL_FreeSurface(image);

  //Return success
  mTexture = newTexture;
  return mTexture != NULL;
}

bool Texture::loadFromRenderedText( string textureText, TTF_Font* fontFamily, SDL_Color textColor ) {
  // Get rid of preexisting texture
  free();

  //Render text surface
  SDL_Surface* textSurface = TTF_RenderText_Solid( fontFamily, textureText.c_str(), textColor );
  if( textSurface != NULL ) { 
    //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if( mTexture == NULL ) { 
      cout << endl << "Unable to create texture from rendered text!: " << SDL_GetError() << endl;
    } else { 
      //Get image dimensions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }
     
    //Get rid of old surface
    SDL_FreeSurface( textSurface );
  } else {
    cout << endl << "Unable to render text surface!: " << TTF_GetError() << endl;
  }

  //Return success
  return mTexture != NULL;
}

void Texture::free() {
  //Free texture if it exists
  if( mTexture != NULL )
  {
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void Texture::render( int x, int y, SDL_Rect* clip ) {
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  //Set clip rendering dimensions
  if( clip != NULL ) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopy( renderer, mTexture, clip, &renderQuad );
}

void Texture::renderWithAngle( int x, int y, SDL_Rect* clip, double angle) {
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  //Set clip rendering dimensions
  if( clip != NULL ) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, NULL, SDL_FLIP_NONE );
}

int Texture::getWidth() {
  return mWidth;
}

int Texture::getHeight() {
  return mHeight;
}
