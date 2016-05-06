#include <comapi.h>
#include <sys/file.h>
#include <stdlib.h>

#define FIFO_MODE 0666

// TODO: Error handling

connection_t * mkconnect(address_t address) {
	FILE * file;
	connection_t * connection;

	if(mkfifo(address.path, FIFO_MODE) == -1) {
		return NULL;
	}

	connection = connect(address);
	if(connection == NULL) {
		unlink(address.path);
		return NULL;
	}

	connection->owner = TRUE;

	return connection;
}

connection_t * connect(address_t address) {
	FILE * file;
	connection_t * connection;

	file = fopen(address.path, "w");
	if(file == NULL) {
		return NULL;
	}

	connection = connection_new();
	if(connection == NULL) {
		return NULL;
	}

	connection->owner = FALSE;
	connection->type = CONNECTION_PIPE;
	connection->address.path = address.path;
	connection->channel.file = file;

	return connection;
}

int pipe_remove(connection_t * connection) {
	// TODO: Validacion de los datos del connection?
	address_t address = connection->address;

	if(!pipe_disconnect(connection->channel.file)) {
		return FALSE;
	}

	if(unlink(address.path) == -1) {
		// TODO: Error handling
		return FALSE;
	}

	return TRUE;
}

int pipe_disconnect(connection_t * connection) {
	if(connection == NULL) {
		// TODO: Errores
		return FALSE;
	}

	// TODO: Validacion de los datos del connection?

	if(fclose(connection->channel.file) == EOF) {
		// TODO: Error handling
		// free_connection_t(connection);?
		return FALSE;
	}

	connection_free(connection);

	return TRUE;
}

typedef struct {
	char * structure;
	struct * value;
} __attribute__((packed)) data_t;
"S(iA($(ii)))"

args_t vars = {};
data_t data = {"S(iA(ii))", vars};

int seriallize(connection_t * connection, data_t * data) {
	tpl_node * node;
	int fd;

	fd = fileno(connection->file);
	node = tpl_map(data.structure, data.content);
	tpl_pack(node, 0);
	tpl_dump(node, TPL_FD, fd);
	tpl_free(node);
}

#define _ERROR_ARGUMENTSINVALID -1
#define _ERROR_LOCK -2
#define _ERROR_DATAINCOMPLETE -3

int pipe_send(connection_t * connection, args_t * data) {
	int i, fd, items, total = 0;
	size_t size, length;

	if(connection == NULL || data == NULL) {
		return _ERROR_ARGUMENTSINVALID;
	}

	fd = fileno(connection->channel.file);
	if(flock(fd, LOCK_EX) == -1) {
		// Error
		// This should never happend
		return _ERROR_LOCK;
	}

	size = sizeof(data->size);
	items = fwrite(&data->size, size, 1, connection->channel.file);
	if(items < 1) {
		// Error
		return _ERROR_DATAINCOMPLETE;
	}
	total += items;

	for(i = 0; i < data->size; i++) {
		size = sizeof(data->vars[i].type);
		items = fwrite(&data->vars[i].type, size, 1, connection->channel.file);
		if(items < 1) {
			// Error
			return _ERROR_DATAINCOMPLETE;
		}
		total += items;

		if(data->vars[i].type == VALUE_STRING) {
			length = strlen(data->vars[i].value.string)
			size = sizeof(length);
			items = fwrite(length, size, 1, connection->channel.file);
			if(items < 1) {
				// Error
				return _ERROR_DATAINCOMPLETE;
			}
			total += items;

			size = sizeof(data->vars[i].value.string[0]);
			items = fwrite(data->vars[i].value.string, size, length, connection->channel.file);
			if(items < length) {
				// Error
				return _ERROR_DATAINCOMPLETE;
			}
			total += items;
		} else {
			size = sizeof(data->vars[i].value);
			items = fwrite(&data->vars[i].type, size, 1, connection->channel.file);
			if(items < 1) {
				// Error
				return _ERROR_DATAINCOMPLETE;
			}
			total += items;
		}
	}

	size = sizeof(total);
	items = fwrite(total, size, 1, connection->channel.file);
	if(items < 1) {
		// Error
		return _ERROR_DATAINCOMPLETE;
	}

	if(flock(fd, LOCK_UN) == -1) {
		// Error
		// This should never happend
		return _ERROR_LOCK;
	}

	return SUCCESS;
}

int pipe_receive(connection_t * connection, request_t * data) {
	int i;
	size_t bytes, size, total = 0;

	if(connection == NULL || data == NULL) {
		return PIPE_ERROR_INVALIDARGUMENTS;
	}

	// if(flock(connection->channel.fd, LOCK_EX) == -1) {
	// 	// Error
	// 	// This should never happen
	// 	return PIPE_ERROR_LOCK;
	// }

	// TODO: Validate data!

	size = sizeof(data->action);
	bytes = read(connection->channel.fd, &data->action, size);
	if(bytes != size) {
		// Error
		return PIPE_ERROR_DATAINCOMPLETE;
	}
	total += bytes;

	size = sizeof(data->argc);
	bytes = read(connection->channel.fd, &data->argc, size);
	if(bytes != size) {
		// Error
		return PIPE_ERROR_DATAINCOMPLETE;
	}
	total += bytes;

	data->argv = malloc(data->argc * sizeof(data_t))
	if(data->argv == NULL) {
		// Error
		return PIPE_ERROR_DATAINCOMPLETE;
	}

	size = sizeof(data_t);
	for(i = 0; i < data->argc; i++) {
		bytes = read(connection->channel.fd, &data->argv[i], size);
		if(bytes != size) {
			// Error
			return PIPE_ERROR_DATAINCOMPLETE;
		}
		total += bytes;
	}

	// if(flock(connection->channel.fd, LOCK_UN) == -1) {
	// 	// Error
	// 	// This should never happend
	// 	return PIPE_ERROR_RELEASELOCK;
	// }

	return SUCCESS;
}
