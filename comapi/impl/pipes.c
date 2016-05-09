#include <comapi.h>
#include <define.h>
#include <minIni.h>
#include <pipes.h>
#include <random.h>
#include <sys/file.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PIPE_PATH_DEFAULT "/tmp/server"
#define PIPE_NAME_SIZE 11
#define PIPE_DIR_SIZE 5
#define PIPE_PATH_SIZE (PIPE_DIR_SIZE + PIPE_NAME_SIZE)

typedef struct {
	pipe_t * read;
	pipe_t * write;
} __attribute__((packed)) network_t;

typedef struct {
	char * address;
} __attribute__((packed)) config_t;

static connection_t mkserver(char * address, ...);
static connection_t cserver(char * address, ...);

static config_t * config_new();
static void config_free(config_t * config);

static network_t * network_new(pipe_t * read, pipe_t * write);
static void network_free(network_t * network);

connection_t server_open(char * config_file) {
	connection_t connection;
	config_t * config;

	config = config_new(config_file);
	if(config == NULL) {
		return NULL;
	}

	printf("Tengo: %s\n", config->address);

	connection = mkserver(config->address);

	config_free(config);

	return connection;
}

void server_close(connection_t connection) {
	network_t * network = (network_t *) connection;

	if(network != NULL) {
		pipe_remove(network->read);
		pipe_close(network->write);
		network_free(network);
	}
}

void server_disconnect(connection_t connection) {
	network_t * network = (network_t *) connection;

	if(network != NULL) {
		pipe_close(network->read);
		pipe_close(network->write);
		network_free(network);
	}
}

connection_t server_accept(connection_t connection) {
	network_t * nnetwork, * network = (network_t *) connection;
	pipe_t * read, * write;
	char write_address[PIPE_PATH_SIZE];
	char read_address[PIPE_PATH_SIZE] = {'/', 't', 'm', 'p', '/'};;

	printf("Pido el write\n");
	pipe_receive(network->read, (void *) write_address, PIPE_PATH_SIZE);
	printf("Tengo el write: %s\n", write_address);

	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		return NULL;
	}

	randstr(read_address + PIPE_DIR_SIZE, PIPE_NAME_SIZE);
	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		pipe_close(write);
		return NULL;
	}
	pipe_send(write, read_address, PIPE_PATH_SIZE);
	printf("Cree el read: %s\n", read_address);

	nnetwork = network_new(read, write);
	if(nnetwork == NULL) {
		pipe_close(write);
		pipe_remove(read);
		return NULL;
	}
	printf("Cree la conexion\n");

	return (connection_t) nnetwork;
}

connection_t server_connect(char * config_file) {
	connection_t connection;
	config_t * config;

	config = config_new(config_file);
	if(config == NULL) {
		return NULL;
	}

	connection = cserver(config->address);

	config_free(config);

	return connection;
}

void server_disconnect(connection_t connection) {
	network_t * network = (network_t *) connection;

	if(network != NULL) {
		pipe_remove(network->read);
		pipe_close(network->write);
		network_free(network);
	}
}

static connection_t mkserver(char * address, ...) {
	network_t * network;
	pipe_t * read;

	pcg32_srandom(time(NULL), (intptr_t)&read);

	read = pipe_make(address, FALSE);
	if(read == NULL) {
		return NULL;
	}

	network = network_new(read, NULL);
	if(network == NULL) {
		pipe_remove(read);
	}

	return (connection_t) network;
}

static connection_t cserver(char * address, ...) {
	pipe_t * read, * write, * conector;
	char write_address[PIPE_PATH_SIZE];
	char read_address[PIPE_PATH_SIZE] = {'/', 't', 'm', 'p', '/'};
	network_t * network;

	pcg32_srandom(time(NULL), (intptr_t)&read);

	conector = pipe_open(address, TRUE);
	if(conector == NULL) {
		return NULL;
	}
	printf("Pude conectar\n");

	randstr(read_address + PIPE_DIR_SIZE, PIPE_NAME_SIZE);
	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		pipe_close(conector);
		return NULL;
	}
	printf("Cree el read: %s\n", read_address);

	pipe_send(conector, (void *) read_address, PIPE_PATH_SIZE);
	pipe_receive(read, (void *) write_address, PIPE_PATH_SIZE);

	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		pipe_close(conector);
		pipe_remove(read);
		return NULL;
	}
	printf("Abri: %s\n", write_address);

	network = network_new(read, write);
	if(network == NULL) {
		pipe_close(conector);
		pipe_remove(read);
		pipe_close(write);
		return NULL;
	}
	printf("Cree la conexion\n");

	return (connection_t) network;
}

static config_t * config_new(const char * file) {
	config_t * config;
	char * address;
	int ret;

	address = malloc(sizeof(char) * PIPE_PATH_SIZE);
	if(address == NULL) {
		return NULL;
	}

	ret = ini_gets("network", "address", PIPE_PATH_DEFAULT, address, PIPE_PATH_SIZE, file);
	if(ret <= 0) {
		free(address);
		return NULL;
	}

	printf("Configured address: %s\n", address);

	config = malloc(sizeof(config_t));
	if(config == NULL) {
		free(address);
		return NULL;
	}

	config->address = address;

	return config;
}

static void config_free(config_t * config) {
	free(config->address);
	free(config);
}

static network_t * network_new(pipe_t * read, pipe_t * write) {
	network_t * network;

	network = malloc(sizeof(network_t));
	if(network == NULL) {
		return NULL;
	}

	network->read = read;
	network->write = write;

	return network;
}

static void network_free(network_t * network) {
	free(network);
}
