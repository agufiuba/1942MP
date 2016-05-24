#ifndef SRC_MODELS_MSG_MSG_H_
#define SRC_MODELS_MSG_MSG_H_

#include <string>

using namespace std;

class Msg {
public:
	Msg(string id, int type, string value) {
		this->id = id;
		this->type = type;
		this->value = value;
	}

	const string& getId() const {
		return id;
	}

	unsigned short int getType() const {
		return type;
	}

	const string& getValue() const {
		return value;
	}
private:
	string id;
	unsigned short int type;
	string value;
};

#endif
