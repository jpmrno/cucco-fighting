#include <comapi.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html

typedef struct {
	char * address;
	int port;
	int channel;
} __attribute__((packed)) network_t;

static connection_t connection_new(char * address, int sockfd, int port_n);
static void connection_rm(connection_t connection);

connection_t c_mkserver(char * address, ...) { // TODO: No necesita el address?
	struct sockaddr_in socket_a;
	int sockfd, port_n;
	va_list args;

	va_start(args, address);
	port_n = va_arg(args, int);
	va_end(args);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		return NULL;
	}

	bzero((char *) &socket_a, sizeof(socket_a));
	socket_a.sin_family = AF_INET;
	socket_a.sin_port = htons(port_n);
	socket_a.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &socket_a, sizeof(socket_a)) < 0) {
		close(sockfd);
		return NULL;
	}

	if(listen(sockfd, 5) == -1) { // TODO: Leer de configuracion el 5 (u otro numero)
		close(sockfd);
		return NULL;
	}

	return connection_new(address, sockfd, port_n);
}

connection_t c_connect(char * address, ...) {
	struct sockaddr_in socket_a;
	int sockfd, port_n;
	va_list args;

	va_start(args, address);
	port_n = va_arg(args, int);
	va_end(args);

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		return NULL;
	}

	bzero((char *) &socket_a, sizeof(socket_a));
	socket_a.sin_family = AF_INET;
	socket_a.sin_port = htons(port_n);

	if(inet_aton(address, &socket_a.sin_addr) != 1) {
		close(sockfd);
		return NULL;
	}

	if(connect(sockfd, (struct sockaddr *) &socket_a, sizeof(socket_a)) < 0) {
		close(sockfd);
		return NULL;
	}

	return connection_new(address, sockfd, port_n);
}

void c_disconnect(connection_t connection) {
	network_t * network = (network_t *) connection;

	close(network->channel);
	connection_rm(connection);
}

connection_t c_accept(connection_t connection) {
	network_t * network = (network_t *) connection;
	struct sockaddr_in socket_a;
	socklen_t socket_a_len;
	int sockfd;

	socket_a_len = sizeof(socket_a);

	sockfd = accept(network->channel, (struct sockaddr *) &socket_a, &socket_a_len);
	if(sockfd == -1) {
		return NULL;
	}

	return connection_new(network->address, sockfd, network->port);
}

// int send(connection_t connection, data_t data) {


// 	return TRUE;
// }

// data_t receive(connection_t connection) {

// }

static connection_t connection_new(char * address, int sockfd, int port_n) {
	network_t * network;

	network = malloc(sizeof(network_t));
	if(network == NULL) {
		close(sockfd);
		return NULL;
	}

	network->address = malloc(sizeof(char) * strlen(address));
	if(network->address == NULL) {
		close(sockfd);
		free(network);
		return NULL;
	}
	strcpy(network->address, address);
	network->port = port_n;
	network->channel = sockfd;

	return (connection_t) network;
}

static void connection_rm(connection_t connection) {
	network_t * network = (network_t *) connection;
	free(network->address);
	free(network);
}
