/*
 * FlotaObserver.h
 *
 *  Created on: 19 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_FLOTAOBSERVER_H_
#define SRC_GAME_MODEL_FLOTAOBSERVER_H_

class FlotaObserver {
private:
	int cfd;
	int cantidadDeAvionesEnLaFlota;
	int contador;
	bool todosMatadosPorUnJugador;


public:
	FlotaObserver();
	~FlotaObserver();
	void actualizar(int cfdQueEliminoAlAvion);
};

#endif /* SRC_GAME_MODEL_FLOTAOBSERVER_H_ */
