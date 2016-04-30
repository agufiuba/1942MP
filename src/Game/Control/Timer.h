#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#ifndef SRC_GAME_CONTROL_TIMER_H_
#define SRC_GAME_CONTROL_TIMER_H_

class Timer{
private:
    int tiempoInicial;
    int tiempoPausado;

    bool paused;
    bool started;

public:
    Timer();

    void correr();
    void parar();
    void pausar();
    void despausar();

    int tiempoActual();

    bool is_started();
    bool is_paused();
};

#endif /* SRC_GAME_CONTROL_TIMER_H_ */
