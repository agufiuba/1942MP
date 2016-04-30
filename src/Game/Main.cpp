#include "Model/Avion.h"
#include "Control/Controller.h"

#include <iostream>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;


SDL_Renderer* renderer;
SDL_Window * screen;
SDL_Event event;

const int FRAMES_PER_SECOND = 10;



using namespace std;

//Clase TIMER--------------------------------------------------

class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
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

// TIMER -----------------------------------------------

bool init(){

    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
//    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
//    SDL_Window *screen = SDL_CreateWindow("1942MP Arcade", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
//                                  SDL_WINDOW_OPENGL);

  	// Create window
    SDL_Window * screen = SDL_CreateWindow("1942MP Arcade", SDL_WINDOWPOS_UNDEFINED,
  	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //If there was an error in setting up the screen
    if( screen == NULL ) {
  		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  		return false;
    }

  	//Create renderer for window
  	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
  	if (renderer == NULL) {
  		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  		return false;
  	}

  	//Initialize renderer color
  	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Set the window caption
//    SDL_WM_SetCaption( "1942MP Arcade", NULL );
    //If everything initialized fine
    return true;
}



int main(int argc, char **argv) {

  if( init() == false ){
  		cout << "No cargo bien el screen"<< endl;
      return 1;
  }

	bool quit = false;

  Timer fps;

  Vivible* unAvion = new Avion(renderer);
  Controller* control = new Controller(unAvion);


  while(!quit){
  		fps.start();

      while( SDL_PollEvent( &event ) ){
          control->press( &event);

          if( event.type == SDL_QUIT ){ quit = true; }
      }


    	//Clear screen
    	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

      //Clear screem
			SDL_RenderClear(renderer);


      control->hacerVivir();

			//Update screen
			SDL_RenderPresent(renderer);

      //cap
      if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ){
          SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
      }
  }

  SDL_Quit();

	delete control;

	return 0;
}
