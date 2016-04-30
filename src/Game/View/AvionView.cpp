#include "AvionView.h"

using namespace std;


//TODO revisar
void AvionView::cargarClips(){
		clipsDerecha[ 0 ].x = 0;
		clipsDerecha[ 0 ].y = 0;
		clipsDerecha[ 0 ].w = anchoVista;
		clipsDerecha[ 0 ].h = largoVista;

		clipsDerecha[ 1 ].x = 0;
		clipsDerecha[ 1 ].y = 0;
		clipsDerecha[ 1 ].w = anchoVista-50;
		clipsDerecha[ 1 ].h = largoVista;

		clipsIzquierda[ 0 ].x = 0;
		clipsIzquierda[ 0 ].y = 0;
		clipsIzquierda[ 0 ].w = anchoVista;
		clipsIzquierda[ 0 ].h = largoVista;

		clipsIzquierda[ 1 ].x = 0;
		clipsIzquierda[ 1 ].y = 0;
		clipsIzquierda[ 1 ].w = anchoVista;
		clipsIzquierda[ 1 ].h = largoVista-50;

		clipsVueltereta[ 0 ].x = 0;
		clipsVueltereta[ 0 ].y = 0;
		clipsVueltereta[ 0 ].w = anchoVista;
		clipsVueltereta[ 0 ].h = largoVista;

		clipsVueltereta[ 1 ].x = 0;
		clipsVueltereta[ 1 ].y = 0;
		clipsVueltereta[ 1 ].w = anchoVista;
		clipsVueltereta[ 1 ].h = largoVista;

		clipsVueltereta[ 2 ].x = 0;
		clipsVueltereta[ 2 ].y = 0;
		clipsVueltereta[ 2 ].w = anchoVista;
		clipsVueltereta[ 2 ].h = largoVista;

		clipsVueltereta[ 3 ].x = 0;
		clipsVueltereta[ 3 ].y = 0;
		clipsVueltereta[ 3 ].w = anchoVista;
		clipsVueltereta[ 3 ].h = largoVista;

		clipsVueltereta[ 4 ].x = 0;
		clipsVueltereta[ 4 ].y = 0;
		clipsVueltereta[ 4 ].w = anchoVista;
		clipsVueltereta[ 4 ].h = largoVista;

		clipsVueltereta[ 5 ].x = 0;
		clipsVueltereta[ 5 ].y = 0;
		clipsVueltereta[ 5 ].w = anchoVista;
		clipsVueltereta[ 5 ].h = largoVista;

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
		anchoVista = 125;
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

void AvionView::mostrar( int x, int y, int velX){
		//cout << "x " << x << "       y "<< y <<endl;

	bool izquierda = true;
	int frame = 0;

/*
  if( velX < 0 ){
      frame++;
  }
  else if( velX > 0 ){
      izquierda = false;
      frame++;

  }else{
      frame = 0;
  }*/

  if (velX > 0){
  	izquierda = false;
  }

  if (velX != 0){
    frame ++;
  }

  SDL_Rect* currentClip;

  if(izquierda){
  	currentClip = &clipsIzquierda[ frame ];
  }else{
  	currentClip = &clipsDerecha[ frame ];
  }

  vistaAvionTexture->render( x, y, rendererAvion, currentClip );
}


