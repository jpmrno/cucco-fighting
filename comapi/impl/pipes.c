#include <comapi.h>
#include <pipes.h>
#include <sys/file.h>
#include <stdlib.h>

typedef struct {
	pipe_t * read;
	pipe_t * write;
} __attribute__((packed)) network_t;

connection_t c_mkserver(char * address, ...) {
	connection_t nconnection;
	pipe_t * read;

	read = pipe_make(address, FALSE);
	if(read == NULL) {
		return NULL;
	}

	nconnection = connection_new(read, NULL);
	if(nconnection == NULL) {
		pipe_remove(read);
	}

	return nconnection;
}

connection_t c_connect(char * address, ...) {
	open();
	read = pipe_make();
	write(read);
	write = receive(read);
	connection_make(read, write);

	return connection;
}

connection_t c_accept(connection_t connection) {
	network_t * network = (network_t *) connection;

	read = pipe_make();
	write = receive(general);
	send(read);
	connection_make(read, write);

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
