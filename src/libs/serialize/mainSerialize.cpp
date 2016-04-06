#include <iostream>
#include "string.h"

using namespace std;

typedef struct Mensaje {
  string id;
  string tipo;
  string valor;
} Mensaje;

void serialize(Mensaje* mensaje, char* data) {
	cout << "serializando.." << endl;

	char *id = new char[sizeof(mensaje->id)];
	char *tipo = new char[sizeof(mensaje->tipo)];
	char *valor = new char[sizeof(mensaje->valor)];

	std::copy(mensaje->id.begin(),mensaje->id.end(), id);
	std::copy(mensaje->tipo.begin(),mensaje->tipo.end(),tipo);
	std::copy(mensaje->valor.begin(),mensaje->valor.end(),valor);

	strcat(data,id);
	strcat(data,"-");
	strcat(data,tipo);
	strcat(data,"-");
	strcat(data,valor);
}

void desSerialize(char* data, Mensaje* mensaje) {
	cout << "desSerializando.." << endl;
	string dataString(data);
	string separador = "-";
	int separadores = 0;

	for (unsigned int i = 0 ; i < dataString.length(); i++) {

		string letra = dataString.substr(i,1);
		if (letra.compare("\0") != 0) {

			if (separadores < 3 && letra.compare(separador) == 0 ) {
				separadores++;
				continue;
			}

			if (separadores == 0) {
				mensaje->id += letra;
			} else if (separadores == 1) {
				mensaje->tipo += letra;
			} else {
				mensaje->valor += letra;
			}
		}
	}
}

int main () {
	Mensaje* mensaje = new Mensaje;
	char* data = new char[sizeof(mensaje)];

	mensaje->id = "1";
	mensaje->tipo = "int";
	mensaje->valor = "Hola mundo, - chau gente";
	serialize(mensaje,data);

	Mensaje* nuevo = new Mensaje;
	desSerialize(data, nuevo);

	cout << "Mensaje nuevo id: " << nuevo->id << endl;
	cout << "Mensaje nuevo tipo: " << nuevo->tipo << endl;
	cout << "Mensaje nuevo valor: " << nuevo->valor << endl;
	return 0;
}
