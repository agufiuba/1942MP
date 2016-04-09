#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#include <string>
#include "../msg/Mensaje.h"
using namespace std;

class Serialize {

	public:
		static string serialize(Mensaje data);
		static Mensaje desSerialize(string serial);

};

#endif /* SERIALIZE_H_ */
