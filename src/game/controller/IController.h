/*
 * IController.h
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

class IController {
public:
	virtual ~IController(){};
	virtual void press(SDL_Event *event) {};
	virtual void hacerVivir() {};
};



#endif /* ICONTROLLER_H_ */
