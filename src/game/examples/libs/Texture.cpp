#include "Texture.h"
#include "string"
#include <iostream>

using namespace std;


Texture::Texture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFromFile( std::string path , SDL_Renderer* &gRenderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* image = NULL;

	image = SDL_LoadBMP( path.c_str() );
	if( image == NULL ) {
	  cout << "Unable to load image.\nSDL error: " << SDL_GetError() << endl;
	} else {
		//Color de la imagen jugador (le saco el fondo)
		SDL_SetColorKey( image, SDL_TRUE, SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, image );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
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

void Texture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render( int x, int y, SDL_Renderer* &gRenderer )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
