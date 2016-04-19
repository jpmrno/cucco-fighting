#include <pipes.h>

connection_t * pipe_make(address_t address) {
	int fd;
	connection_t * connection;

	if(mkfifo(address.path, 0666) == -1) { // TODO: define 0666
		// TODO: Error handling
		return NULL;
	}

	fd = open(address.path, O_RDWR); // TODO: fopen o open?

	if(fd == -1) {
		// TODO: Error handling
		unlink(address.path);
		return NULL;
	}

	connection = connection_new();
	if(connection == NULL) {
		return NULL;
	}

	connection->type = CONNECTION_PIPE;
	connection->address.path = address.path;
	connection->channel.fd = fd;

	return connection;
}

connection_t * pipe_connect(address_t address) {
	int fd;
	connection_t * connection;

	fd = open(path, O_RDWR); // TODO: fopen o open?

	if(fd == -1) {
		// TODO: Error handling
		return NULL;
	}

	connection = connection_new();
	if(connection == NULL) {
		return NULL;
	}

	connection->type = CONNECTION_PIPE;
	connection->address.path = address.path;
	connection->channel.fd = fd;

	return connection;
}

int pipe_remove(connection_t * connection) {
	if(connection == NULL) {
		// TODO: Errores
		return FALSE;
	}

	// TODO: Validacion de los datos del connection

	if(close(connection->channel.fd) == -1) {
		// TODO: Error handling
		// free_connection_t(connection);?
		return FALSE;
	}

	if(unlink(connection->address.path) == -1) {
		// TODO: Error handling
		// free_connection_t(connection);?
		return FALSE;
	}

	connection_free(connection);

	return TRUE;
}

int pipe_disconnect(connection_t * connection) {
	if(connection == NULL) {
		// TODO: Errores
		return FALSE;
	}

	// TODO: Validacion de los datos del connection?

	if(close(connection->channel.fd) == -1) {
		// TODO: Error handling
		// free_connection_t(connection);?
		return FALSE;
	}

	connection_free(connection);

	return TRUE;
}

