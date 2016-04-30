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
	vistaAvionTexture = new Texture();

	if (rendererAvion == NULL){
		printf("nulo");
	}

	if (!vistaAvionTexture->loadFromFile(filename, rendererAvion)) {
		printf("Failed to load avion texture image!\n");
		return false;
	}

    return true;
}

AvionView::AvionView(SDL_Renderer * unRenderer){
	//TODO pasarlo a sprites
		anchoVista = 120;
		largoVista = 120;

	cargarClips();

	rendererAvion = unRenderer;

	if (!cargarImagenDelAvion("images/naveEspacial.bmp")) {
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
	vistaAvionTexture->free();
}

void AvionView::mostrar( int x, int y){
		//cout << "x " << x << "       y "<< y <<endl;
    SDL_Rect* currentClip = &clipsDerecha[ 0 ];
    vistaAvionTexture->render( x, y, rendererAvion, currentClip );
}


