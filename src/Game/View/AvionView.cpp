#include "AvionView.h"

using namespace std;

void AvionView::cargarClips(){
		clipsDerecha[ 0 ].x = 0;
		clipsDerecha[ 0 ].y = 0;
		clipsDerecha[ 0 ].w = anchoVista;
		clipsDerecha[ 0 ].h = largoVista;

		clipsIzquierda[ 0 ].x = 0;
		clipsIzquierda[ 0 ].y = 0;
		clipsIzquierda[ 0 ].w = anchoVista;
		clipsIzquierda[ 0 ].h = largoVista;
}

SDL_Surface *AvionView::cargarImagenDelAvion( string filename ){
    SDL_Surface* imagenCargada = NULL;
    SDL_Surface* imagenOptimizada = NULL;
    imagenCargada = IMG_Load( filename.c_str() );

    if( imagenCargada != NULL ){
        imagenOptimizada = SDL_DisplayFormat( imagenCargada );

        SDL_FreeSurface( imagenCargada );

        if( imagenOptimizada != NULL ){
            SDL_SetColorKey( imagenOptimizada, SDL_SRCCOLORKEY, SDL_MapRGB( imagenOptimizada->format, 0, 0xFF, 0xFF ) );
        }
    }
    return imagenOptimizada;
}

AvionView::AvionView(SDL_Surface * unEscenario){

	anchoVista = 100;
	largoVista = 100;

	cargarClips();

	screen = unEscenario;

	vistaAvion = cargarImagenDelAvion("images/Arcade_-_1942_-_General_Sprites.bmp");

	if (vistaAvion == NULL) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
		//TODO: faltaria que tire una excepcion
	}
}

int AvionView::getAnchoVista(){
		return anchoVista;
}

int AvionView::getLargoVista(){
		return largoVista;
}

AvionView::~AvionView(){
		SDL_FreeSurface( vistaAvion );
}

void AvionView::cargarmeEnEscenario( int x, int y, SDL_Surface* obj, SDL_Surface* escenario, SDL_Rect* clip){
    //Holds offsets
    SDL_Rect posicion;

    //Get offsets
    posicion.x = x;
    posicion.y = y;

    //Blit
    SDL_BlitSurface( obj, clip, escenario, &posicion );
}

void AvionView::mostrar(int x, int y){

		cargarmeEnEscenario( x, y, vistaAvion, screen, &clipsDerecha[0] );
}



