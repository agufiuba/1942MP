/*
 * IController.h
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

#include "../model/Avion.h"
#include "../controller/ControllerMissiles.h"

class IController {
public:
	virtual ~IController(){};
	virtual void press(SDL_Event *event) {};
	virtual void hacerVivir() {};
	virtual Vivible* getVivible(){return 0;};
	virtual void setAmetralladora() {};
	virtual void setBalaNormal() {};
	virtual ControllerMissiles* getMissiles(){return 0;}
};



#endif /* ICONTROLLER_H_ */
