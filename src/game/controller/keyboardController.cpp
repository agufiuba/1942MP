#include "keyboardController.h"

using namespace std;

keyboardController::keyboardController() {
	this->isRunningGame = true;

}

keyboardController::~keyboardController() {

}

void keyboardController::setIsRunningGame(bool isRunninggame){
	this->isRunningGame = isRunninggame;
}
