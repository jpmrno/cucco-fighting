#include <comapi.h>
#include <define.h>
#include <pipes.h>
#include <sys/file.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	pipe_t * read;
	pipe_t * write;
} __attribute__((packed)) network_t;

static connection_t connection_new(pipe_t * read, pipe_t * write);
static void connection_free(connection_t connection);
static void randstr(char *s, const int len);

connection_t c_mkserver(char * address, ...) {
	srand(time(NULL));
	connection_t connection;
	pipe_t * read;

	read = pipe_make(address, FALSE);
	if(read == NULL) {
		return NULL;
	}

	connection = connection_new(read, NULL);
	if(connection == NULL) {
		pipe_remove(read);
	}

	return connection;
}

#define PIPENAME_SIZE 11
connection_t c_connect(char * address, ...) {
	srand(time(NULL));
	pipe_t * read, * write, * conector;
	char write_address[PIPENAME_SIZE+5], read_address[PIPENAME_SIZE+5];
	connection_t connection;

	printf("Intento conectar\n");
	conector = pipe_open(address, TRUE);
	if(conector == NULL) {
		printf("Error al conectar\n");
		return NULL;
	}
	printf("Pude conectar\n");

	read_address[0] = '/';
	read_address[1] = 't';
	read_address[2] = 'm';
	read_address[3] = 'p';
	read_address[4] = '/';
	randstr(read_address+5, PIPENAME_SIZE);
	printf("Intento crear el read: %s\n", read_address);
	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		printf("No pude crear el read: %s\n", read_address);
		pipe_remove(conector);
		return NULL;
	}
	printf("Pude crear el read, lo intento mandar: %s\n", read_address);

	pipe_send(conector, (void *) read_address, PIPENAME_SIZE+5);
	printf("Intento recibir el write\n");
	pipe_receive(read, (void *) write_address, PIPENAME_SIZE+5);
	printf("Pude recibir el write, intento abrir: %s\n", write_address);

	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		printf("No pude abrir: %s\n", write_address);
		pipe_remove(conector);
		pipe_remove(read);
		return NULL;
	}
	printf("Pude abrir: %s\n", write_address);

	printf("Creo la conexion\n");
	connection = connection_new(read, write);
	if(connection == NULL) {
		printf("Error al crear la conexion\n");
		pipe_remove(conector);
		pipe_remove(write);
		pipe_remove(read);
		return NULL;
	}
	printf("Pude crear la conexion\n");

	return connection;
}

void c_disconnect(connection_t connection) {
	connection_free(connection);
}

connection_t c_accept(connection_t connection) {
	network_t * network = (network_t *) connection;
	pipe_t * read, * write;
	char write_address[PIPENAME_SIZE+5], read_address[PIPENAME_SIZE+5];
	connection_t nconnection;

	printf("Pido el write\n");
	pipe_receive(network->read, (void *) write_address, PIPENAME_SIZE+5);
	printf("Me dio el write: %s\n", write_address);

	printf("Intento abrir el write: %s\n", write_address);
	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		printf("Error en el write\n");
		return NULL;
	}
	printf("Pude abrir el write: %s\n", write_address);

	read_address[0] = '/';
	read_address[1] = 't';
	read_address[2] = 'm';
	read_address[3] = 'p';
	read_address[4] = '/';
	randstr(read_address+5, PIPENAME_SIZE);
	printf("Intento crear el read: %s\n", read_address);
	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		printf("No pude crear el read: %s\n", read_address);
		pipe_remove(write);
		return NULL;
	}
	printf("Pude crear el read, lo intento mandar: %s\n", read_address);

	pipe_send(write, read_address, PIPENAME_SIZE+5);
	printf("Pude mandar el read: %s\n", read_address);

	printf("Creo la conexion\n");
	nconnection = connection_new(read, write);
	if(nconnection == NULL) {
		printf("Error al crear la conexion\n");
		pipe_remove(write);
		pipe_remove(read);
		return NULL;
	}
	printf("Pude crear la conexion\n");

	return nconnection;
}

static connection_t connection_new(pipe_t * read, pipe_t * write) {
	network_t * network;

	network = malloc(sizeof(network_t));
	if(network == NULL) {
		return NULL;
	}

	network->read = read;
	network->write = write;

	return (connection_t) network;
}

static void connection_free(connection_t connection) {
	network_t * network = (network_t *) connection;

	if(network != NULL) {
		pipe_remove(network->read);
		pipe_remove(network->write);
		free(network);
	}
}

static void randstr(char * s, const int length) {
	int i;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for(i = 0; i < length - 1; i++) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[length] = 0;
}
