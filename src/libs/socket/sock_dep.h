#include <sys/socket.h> /* socket, bind, listen, accept, connect, close */
#include <sys/types.h> /* socket, bind, listen, accept, connect, close */
#include <netinet/in.h> /* IPv4 & IPv6 presentation strings max.length constants (INET6?_ADDRSTRLEN) */
#include <netdb.h> /* hostent, addrinfo */
#include <arpa/inet.h> /* inet_ntop() */
#include <unistd.h> /* close() */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(), bzero() */
