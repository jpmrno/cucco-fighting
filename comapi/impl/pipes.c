#include <comapi.h>
#include <pipes.h>
#include <sys/file.h>
#include <stdlib.h>

typedef struct {
	pipe_t * read;
	pipe_t * write;
} __attribute__((packed)) network_t;

connection_t c_mkserver(char * address, ...) {
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

connection_t c_connect(char * address, ...) {
	pipe_t * read, write, conector;
	char * read_address, write_address;
	connection_t connection;

	conector = pipe_open(address, TRUE);
	if(conector == NULL) {
		return NULL;
	}

	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		pipe_remove(conector);
		return NULL;
	}

	//pipe_send(conector, read_address);
	//write_address = pipe_receive(read);

	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		pipe_remove(conector);
		pipe_remove(read);
		return NULL;
	}

	connection = connection_make(read, write);
	if(connection == NULL) {
		pipe_remove(conector);
		pipe_remove(read);
		pipe_remove(write);
		return NULL;
	}

	return connection;
}

connection_t c_accept(connection_t connection) {
	network_t * network = (network_t *) connection;
	pipe_t * read, write, conector;
	char * read_address, write_address;
	connection_t connection;

	//write_address = pipe_receive(connection->read);

	write = pipe_open(write_address, TRUE);
	if(write == NULL) {
		return NULL;
	}

	read = pipe_make(read_address, FALSE);
	if(read == NULL) {
		pipe_remove(write);
		return NULL;
	}

	//pipe_send(write, read_address);

	connection = connection_make(read, write);
	if(connection == NULL) {
		pipe_remove(write);
		pipe_remove(read);
		return NULL;
	}

	return connection;
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

static void connection_remove(connection_t connection) {
	network_t * network = (network_t *) connection;

	pipe_remove(network->read);
	pipe_remove(network->write);
	free(network);
}
