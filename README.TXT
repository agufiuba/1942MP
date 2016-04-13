Taller de Programación I - 2016 - 1er Cuatrimestre - TP 1

Integrantes:
- CENTURIÓN, Ramón				94092
- NAKASONE, Nicolas Cristian			93632
- VALLERINO, Gonzalo Adrián			91366
- SCOCCIMARRO, Maximiliano			93784
- GAILLARD, Agustín				94849

Sistema Operativo: 
- Linux

Bibliotecas utlilizadas:
- TinyXML-2	https://github.com/leethomason/tinyxml2

C++ 4.9
- queue
- mutex
- thread
- regex
- vector

Compilacion y Ejecución del proyecto:

-Ir mediante la terminal de Linux hasta la ruta del proyecto

-Una vez alli, se puede compilar el cliente y/o el servidor usando el makefile:
	.cliente: make p=client
	.server: make p=server
Aclaracion: para la compilacion se necesita tener el compilador gcc 4.9 y se compila manualmente con: 
	gcc -std=c++11 -pthread -Wno-write-strings

-Si no hubo errores, se puede ejecutar el proyecto de la siguiente manera:
	.cliente: make p=client arch=nombreXMLConfiguracionCliente run
	.server: make p=server arch=nombreArchivoConfiguracionServer run

