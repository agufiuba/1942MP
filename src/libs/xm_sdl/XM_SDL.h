#ifndef XM_SDL_H
#define XM_SDL_H
#include <SDL2/SDL.h>
#include <vector>

class XM_SDL {
  private:
    SDL_Window* window; 
    SDL_Surface* windowBG;
    std::vector<SDL_Surface*> loadedMedia;
    SDL_Surface* getWindowSurface();
    void updateWindowSurface();
    SDL_Surface* loadImage( const char* IMAGE_PATH );
    bool optimizeSurface( SDL_Surface*& surface );

  public:
    XM_SDL( uint32_t flags );
    ~XM_SDL();
    bool createWindow( const char* TITLE, const int WIDTH = 640, const int HEIGHT = 480 );
    void setWindowBG( const uint8_t RED, const uint8_t GREEN, const uint8_t BLUE );
    void setWindowBG( const char* IMAGE_PATH );
    SDL_Event nextEvent();
};
#endif
