#include <comapi.h>
#include <library.h>
#include <pipes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

static connection_t * connection_new(int new, ctype_t type, address_t address);
static int connection_remove(int owner, connection_t * connection);

connection_t * mkconnect(ctype_t type, address_t address) {
	return mkconnection(TRUE, type, address);
}

int rmconnect(connection_t * connection) {
	return rmconnection(TRUE, connection);
}

connection_t * connect(ctype_t type, address_t address) {
	return mkconnection(FALSE, type, address);
}

connection_t * disconnect(ctype_t type, address_t address) {
	return rmconnection(FALSE, type, address);
}

static connection_t * connection_new(int new, ctype_t type, address_t address) {
	int ret;

	switch(type) {
		case CONNECTION_PIPE: {
			if(new) {
				ret = pipe_make(address);
			} else {
				ret = pipe_connect(address);
			}
		} break;

		case CONNECTION_SOCKET: {
			;
		} break;
	}

	if(!ret) {
		// TODO: Errores
		return NULL;
	}

	return connection;
}

static int connection_remove(int owner, connection_t * connection) {
	int ret;

	switch(connection->type) {
		case CONNECTION_PIPE: {
			if(owner) {
				ret = pipe_remove(connection);
			} else {
				ret = pipe_disconnect(connection);
			}
		} break;

		case CONNECTION_SOCKET: {
			;
		} break;
	}

	if(!ret) {
		// TODO: Errores
		return FALSE;
	}

	return TRUE;
}
