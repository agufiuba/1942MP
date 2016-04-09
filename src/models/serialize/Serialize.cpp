#include "Serialize.h"
using namespace std;

string Serialize::serialize(Mensaje data) {

	string separador = "-";
	string id = data.id;
	string tipo = data.tipo;
	string valor = data.valor;

	return (id + separador + tipo + separador + valor);
}
Mensaje Serialize::desSerialize(string serial) {

	Mensaje mensaje;
	string separador = "-";
	int separadores = 0;

	for (unsigned int i = 0 ; i < serial.length(); i++) {

				string letra = serial.substr(i,1);
				if (letra.compare("\0") != 0) {

					if (separadores < 3 && letra == separador ) {
						separadores++;
						continue;
					}

					if (separadores == 0) {
						mensaje.id += letra;
					} else if (separadores == 1) {
						mensaje.tipo += letra;
					} else {
						mensaje.valor += letra;
					}
				}
	}


	return mensaje;
}
