#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include "Texture.h"
using namespace std;

class Screen {
  private:
    SDL_Renderer* renderer;
    map<string, Texture*>* textures;
    map<string, SDL_Rect>* figures;

  public:
    Screen( SDL_Renderer* renderer );
    ~Screen();
    void loadTexture( string id, string path );
};
#endif
