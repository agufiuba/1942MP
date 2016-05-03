#ifndef SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_
#define SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

using namespace std;

//Texture wrapper class
class Texture{
	public:
		//Initializes variables
		Texture();

		//Deallocates memory
		~Texture();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* &renderer );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y , SDL_Renderer* &renderer, SDL_Rect* clip);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif /* SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_ */
