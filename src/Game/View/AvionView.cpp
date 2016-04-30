#include "AvionView.h"

using namespace std;


//TODO revisar
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

bool AvionView::cargarImagenDelAvion( string filename ){
//    SDL_Surface* imagenCargada = NULL;
//    SDL_Surface* imagenOptimizada = NULL;
//    imagenCargada = IMG_Load( filename.c_str() );
//
//    if( imagenCargada != NULL ){
//        imagenOptimizada = SDL_ConvertSurfaceFormat( imagenCargada , SDL_PIXELFORMAT_RGB24, 0);
//
//        SDL_FreeSurface( imagenCargada );
//
//        if( imagenOptimizada != NULL ){
//            SDL_SetColorKey( imagenOptimizada, SDL_SRCCOLORKEY, SDL_MapRGB( imagenOptimizada->format, 0, 0xFF, 0xFF ) );
//        }
//    }

	vistaAvionTexture = new Texture();

	if (rendererAvion == NULL){
		printf("nulo");
	}

	if (!vistaAvionTexture->loadFromFile(filename, rendererAvion)) {
		printf("Failed to load aviontexture image!\n");
		return false;
	}

    return true;
}

AvionView::AvionView(SDL_Renderer * unRenderer){

	cargarClips();

	rendererAvion = unRenderer;

	if (!cargarImagenDelAvion("images/naveEspacial.bmp")) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
		//TODO: faltaria que tire una excepcion
	}
	//TODO pasarlo a sprites
		anchoVista = vistaAvionTexture->getWidth();
		largoVista = vistaAvionTexture->getHeight();
}

int AvionView::getAnchoVista(){
		return anchoVista;
}

int AvionView::getLargoVista(){
		return largoVista;
}

AvionView::~AvionView(){
	vistaAvionTexture->free();
}

void AvionView::mostrar( int x, int y){
    //Holds offsets
    SDL_Rect posicion;

    //Get offsets
    posicion.x = x;
    posicion.y = y;

    SDL_Rect* currentClip = &clipsDerecha[ 0 ];
    vistaAvionTexture->render( ( SCREEN_WIDTH - currentClip->w ) / 2, ( SCREEN_HEIGHT - currentClip->h ) / 2, rendererAvion, currentClip );
}


