#ifndef SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_
#define SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <string>
using namespace std;

//Texture wrapper class
class Texture{
  public:
    //Initializes variables
    Texture( SDL_Renderer* renderer );

    //Deallocates memory
    ~Texture();

    //Loads image at specified path
    bool loadFromFile( string path ); 

    // Creates image from font string
    bool loadFromRenderedText( string textureText, TTF_Font* fontFamily, SDL_Color textColor ); 

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL );

    //Gets image dimensions
    int getWidth();
    int getHeight();

  private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    SDL_Renderer* renderer;
    //Image dimensions
    int mWidth;
    int mHeight;
    string DESCONOCIDO = "src/game/images/desconocido.bmp";
    string DIR_IMAGE = "src/game/images/";
};

#endif /* SRC_GAME_EXAMPLES_LIBS_TEXTURE_H_ */
