#ifndef SRC_GAME_CONTROLER_KEYBOARDCONTROLLER_H_
#define SRC_GAME_CONTROLER_KEYBOARDCONTROLLER_H_

using namespace std;

class keyboardController {
	public:
		keyboardController();
		virtual ~keyboardController();
		void setIsRunningGame(bool isRunningGame);
	private:
		bool isRunningGame;
};

#endif /* SRC_GAME_CONTROLER_KEYBOARDCONTROLLER_H_ */
