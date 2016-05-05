#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include "../../libs/xm_sdl/XM_SDL.h"
#include "Texture.h"
using namespace std;

class Screen {
  private:
    XM_SDL* sdlHandler;
    SDL_Renderer* renderer;
    map<string, Texture*>* textures;
    map<string, SDL_Rect>* figures;
    TTF_Font* fontFamily;
    SDL_Color fontColor;
    void initTextures();
    void addTexture( string id, Texture* texture );

  public:
    Screen( XM_SDL* sdlHandler );
    ~Screen();
    void loadTexture( string id, string path );
    void loadText( string id, string value );
    void loadFont( const char* FONT_PATH );
};
#endif
