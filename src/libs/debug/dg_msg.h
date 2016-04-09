#define DEBUG_PRINT(X) if(DEBUG) std::cout << std::endl << X << std::endl
#define DEBUG_WARN(X) if(DEBUG) std::cout << std::endl << warning(X) << std::endl
#define DEBUG_NOTICE(X) if(DEBUG) std::cout << std::endl << notice(X) << std::endl

// client
#define CONNECTION_LOST "Se perdio la conexion con el servidor."
#define CONNECTION_REJECTED "Has sido rechazado por Max cantidad de clientes."
#define CONNECTION_CLOSE "Se cerro la conexion con el servidor."
#define CONNECTION_ACTIVE "Ya hay una conexion activa." // server - client
#define CONNECTION_RETRY "Error de conexion. Reintentando conexion..."
#define CONNECTION_NOT_ACTIVE "No hay una conexion activa." // server - client
#define CLIENT_CLOSE "Cerrando el cliente..."
#define SENT_DATA(X) "Se envio " + notice(X) + " al servidor"
#define CONNECTION_ERROR(X) warning("No se pudo establecer una conexion con el servidor: ") \
          + X + "\nIntente nuevamente mas tarde." 
#define CONNECTION_SUCCESS(X) notice("Se establecio una conexion con: ") + X
#define RECV_FAIL "No se pudo recibir el mensaje."
#define SEND_FAIL "No se pudo enviar el mensaje." // server - client
#define SEND_CERROR "Debe estar conectado para poder enviar un mensaje."
#define SOCKET_ERROR "No se pudo crear el socket." // server - client
#define SERVER_MSG(X) "Se recibio del servidor el mensaje: " + notice(X)

// server
#define SERVER_DISCONNECT "Desconectando servidor..."
#define SERVER_CLOSE "Cerrando el servidor..."
#define BIND_ERROR "No se pudo hacer bind del socket. Probando siguiente."
#define BIND_CERROR "No se pudo hacer bind para ninguna configuracion del socket."
#define SOCKET_REUSE_ERROR "No se pudo hacer reutilizable al socket."
#define LISTEN_ERROR "No se pudo poner a escuchar al socket."
#define SERVER_START "Se ha iniciado el servidor. Esperando conexiones..."
