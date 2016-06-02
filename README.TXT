TALLER DE PROGRAMACIÓN I - 2016 - 1er CUATRIMESTRE - TP 2

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
- SDL2		sudo get-apt install libsdl2-dev
- SDL_ttf	sudo get-apt install libsdl2-ttf-dev

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
	gcc -std=c++11 -lSDL2 -lX11 -lSDL2_ttf -pthread -Wno-write-strings

EJECUCIÓN
- Si no hubo errores en la compilación, se puede ejecutar el proyecto de la siguiente manera:
	.cliente: make p=client run
	.servidor: make p=server run
	
- Ejecucion Server:
	Se ingresa: 	opcion 1 - iniciar el servidor
 			opcion 2 - cierra el servidor

- Ejecucion Cliente: 
	Se ingresa la IP y el Puerto del servidor y se selecciona Conectar
	Sale pantalla de Conectado o No se pudo conectar:	* Seleccionas continuar o volver depende el caso
	Luego se ingresa en el Modo de Eleccion de Avion: 	* Se ingresa id del cliente y seleccionas un avion entre los 4
								* Seleccionas Continuar y se inicia el juego cuando todos los clientes esten conectados
	Ahora a Jugar!!
