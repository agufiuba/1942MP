TALLER DE PROGRAMACIÓN I - 2016 - 1er CUATRIMESTRE - TP 1

INTEGRANTES
- CENTURIÓN, Ramón				94092
- NAKASONE, Nicolas Cristian			93632
- VALLERINO, Gonzalo Adrián			91366
- SCOCCIMARRO, Maximiliano			93784
- GAILLARD, Agustín				94849

SISTEMA OPERATIVO
- Linux

BIBLIOTECAS UTILIZADAS
- TinyXML-2	https://github.com/leethomason/tinyxml2

C++ 4.9
- queue
- mutex
- thread
- regex
- vector
- map

COMPILACIÓN
- Ir mediante la terminal de Linux hasta la ruta del proyecto

- Una vez allí, se puede compilar el cliente y/o el servidor mediante el makefile:
	.cliente: make p=client
	.servidor: make p=server
	
  Aclaracion: para la compilacion se necesita tener el compilador gcc 4.9 y se compila manualmente con: 
	gcc -std=c++11 -pthread -Wno-write-strings

EJECUCIÓN
- Si no hubo errores en la compilación, se puede ejecutar el proyecto de la siguiente manera:
	.cliente: make p=client run
	.servidor: make p=server run
	
  En el caso que se desee ejecutar con los archivos de configuración xml, se debe realizar:
	.cliente: make p=client arch=nombreXMLConfiguracionCliente run
	.servidor: make p=server arch=nombreXMLConfiguracionServidor run
 	
