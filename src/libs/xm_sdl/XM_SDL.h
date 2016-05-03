#ifndef XM_SDL_H
#define XM_SDL_H
#include <SDL2/SDL.h>
#include <vector>

class XM_SDL {
  private:
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_Surface* windowBG;
    std::vector<SDL_Texture*> loadedTextures;
    SDL_Texture* loadImage( const char* IMAGE_PATH );
    SDL_Texture* createTexture( SDL_Surface*& surface );

  public:
    XM_SDL( uint32_t flags );
    ~XM_SDL();
    bool createWindow( const char* TITLE, const int WIDTH = 800, const int HEIGHT = 600 );
    void setWindowBG( const uint8_t RED, const uint8_t GREEN, const uint8_t BLUE );
    void setWindowBG( const char* IMAGE_PATH );
    SDL_Renderer* getRenderer();
    bool nextEvent( SDL_Event* e );
    void updateWindow();
};
#endif
