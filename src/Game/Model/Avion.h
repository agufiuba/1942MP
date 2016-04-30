#ifndef SRC_MODEL_AVION_H_
#define SRC_MODEL_AVION_H_

#include "../View/AvionView.h"
#include "Vivible.h"

class Avion: public Vivible {
private:
	int anchoFondo, largoFondo; //TODO: esto estaria bueno poder sacarlo del screen

	AvionView* vistaAvion;
	int x, y;

	bool realizandoVueltereta;
	int velocidadEnVueltereta;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	Avion(SDL_Renderer * &renderer);
	~Avion();

	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();

	void vivir(int velX, int velY);
};

#endif /* SRC_MODEL_AVION_H_ */
