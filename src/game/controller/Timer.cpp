#include "Timer.h"


Timer::Timer(){
    tiempoInicial = 0;
    tiempoPausado = 0;
    paused = false;
    started = false;
}

void Timer::correr(){
    started = true;
    paused = false;

    tiempoInicial = SDL_GetTicks();
}

void Timer::parar(){
    started = false;

    paused = false;
}

void Timer::pausar(){
    if(started && !paused){
        paused = true;

        tiempoPausado = SDL_GetTicks() - tiempoInicial;
    }
}

void Timer::despausar(){
    if(paused){
        paused = false;

        tiempoInicial = SDL_GetTicks() - tiempoPausado;

        tiempoPausado = 0;
    }
}

int Timer::tiempoActual(){
    if(started){
        if( paused == true ) {
        		return tiempoPausado;
        }
        else {
            return SDL_GetTicks() - tiempoInicial;
        }
    }
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
