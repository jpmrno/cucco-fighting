#if defined(SERVER) || defined(CLIENT)

#include <comapi.h>
#include <define.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <minIni.h>

// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html

// TODO: Remove printfs!

#define IP_SIZE_MAX 13
#define IP_DEFAULT "127.0.0.1"
#define PORT_DEFAULT 8080

typedef struct {
	char * address;
	int port;
	int channel;
} __attribute__((packed)) network_t;

typedef struct {
	char * ip;
	int port;
} __attribute__((packed)) config_t;

static connection_t mkserver(char * address, ...);
static connection_t cserver(char * address, ...);
static void dserver(network_t * network);

static config_t * config_new();
static void config_free(config_t * config);

static network_t * network_new(char * address, int sockfd, int port_n);
static void network_free(network_t * connection);

// ---[ Server ]---------------------------------------
#ifdef SERVER

connection_t server_open(const char * config_file) {
	connection_t connection;
	config_t * config;

	config = config_new(config_file);
	if(config == NULL) {
		return NULL;
	}

	printf("Tengo: %s:%d\n", config->ip, config->port);

	connection = mkserver(config->ip, config->port);

	config_free(config);

	return connection;
}

void server_close(connection_t connection) {
	dserver((network_t *) connection);
}

void server_ajar(connection_t connection) {
	dserver((network_t *) connection);
}

connection_t server_accept(connection_t connection) {
	network_t * nnetwork, * network = (network_t *) connection;
	struct sockaddr_in socket_a;
	socklen_t socket_a_len = sizeof(socket_a);
	int sockfd;

	sockfd = accept(network->channel, (struct sockaddr *) &socket_a, &socket_a_len);
	if(sockfd == -1) {
		return NULL;
	}

	nnetwork = network_new(network->address, sockfd, network->port);
	if(nnetwork == NULL) {
		close(sockfd);
		return NULL;
	}

	return (connection_t) nnetwork;
}

#endif
// ----------------------------------------------------

// ---[ Client ]---------------------------------------
#ifdef CLIENT

connection_t server_connect(const char * config_file) {
	connection_t connection;
	config_t * config;

	config = config_new(config_file);
	if(config == NULL) {
		return NULL;
	}

	connection = cserver(config->ip, config->port);

	config_free(config);

	return connection;
}

void server_disconnect(connection_t connection) {
	dserver((network_t *) connection);
}

#endif
// ----------------------------------------------------

// ---[ Common ]---------------------------------------
int server_send(connection_t connection, const void * data, size_t size) {
	network_t * network = (network_t *) connection;

	return write(network->channel, data, size);
}

int server_receive(connection_t connection, void * data, size_t size) {
	network_t * network = (network_t *) connection;

	return read(network->channel, data, size);
}
// ----------------------------------------------------

static connection_t mkserver(char * address, ...) {
	network_t * network;
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

	network = network_new(address, sockfd, port_n);
	if(network == NULL) {
		close(sockfd);
		return NULL;
	}

	return (connection_t) network;
}

static connection_t cserver(char * address, ...) {
	network_t * network;
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

	network = network_new(address, sockfd, port_n);
	if(network == NULL) {
		close(sockfd);
		return NULL;
	}

	return (connection_t) network;
}

static void dserver(network_t * network) {
	if(network != NULL) {
		close(network->channel);
		network_free(network);
	}
}

static config_t * config_new(const char * file) {
	config_t * config;
	char * ip;
	int port, ret;

	ip = malloc(sizeof(char) * IP_SIZE_MAX);
	if(ip == NULL) {
		return NULL;
	}

	ret = ini_gets("network", "ip", IP_DEFAULT, ip, IP_SIZE_MAX, file);
	if(ret <= 0) {
		free(ip);
		return NULL;
	}

	port = (int) ini_getl("network", "port", PORT_DEFAULT, file);
	if(ret <= 0) { // TODO: Validar numero de puerto?
		free(ip);
		return NULL;
	}

	printf("Configured address: %s:%d\n", ip, port);

	config = malloc(sizeof(config_t));
	if(config == NULL) {
		free(ip);
		return NULL;
	}

	config->ip = ip;
	config->port = port;

	return config;
}

static void config_free(config_t * config) {
	free(config->ip);
	free(config);
}

static network_t * network_new(char * address, int sockfd, int port_n) {
	network_t * network;

	network = malloc(sizeof(network_t));
	if(network == NULL) {
		return NULL;
	}

	network->address = malloc(sizeof(char) * strlen(address));
	if(network->address == NULL) {
		free(network);
		return NULL;
	}

	strcpy(network->address, address);
	network->port = port_n;
	network->channel = sockfd;

	return network;
}

static void network_free(network_t * network) {
	if(network != NULL) {
		free(network->address);
		free(network);
	}
}

#endif