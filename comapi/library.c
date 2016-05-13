#include <library.h>
#include <define.h>
#include <comapi.h>
#include <tpl.h>
#include <stdlib.h>
#include <stdio.h> // TODO: Remove

static int send(connection_t connection, void * buffer, int size);
static int receive(connection_t connection, void ** buffer, int * size);

int write_i(connection_t connection, int value) {
	void * buffer;
	int size, ret;

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &value) == -1) {
		printf("TPLS ERROR\n");
		return ERROR_TPL;
	}

	ret = send(connection, buffer, size);

	free(buffer);

	return ret;
}

int read_i(connection_t connection, int * value) {
	tpl_node * node;
	void * buffer;
	int size, ret;

	ret = receive(connection, &buffer, &size);
	if(ret < OK) {
		return ret;
	}

	node = tpl_map("i", value);
	if(tpl_load(node, TPL_MEM | TPL_UFREE, buffer, size) == -1) {
		printf("TPLR ERROR\n");
		return ERROR_TPL;
	}
	tpl_unpack(node, 0);
	tpl_free(node);

	return OK;
}

int write_sf(connection_t connection, char * string, double value) {
	void * buffer;
	int size, ret;

	if(tpl_jot(TPL_MEM, &buffer, &size, "sf", &string, &value) == -1) {
		printf("TPLS ERROR\n");
		return ERROR_TPL;
	}

	ret = send(connection, buffer, size);

	free(buffer);

	return ret;
}

int read_sf(connection_t connection, char ** string, double * value) {
	tpl_node * node;
	void * buffer;
	int size, ret;

	ret = receive(connection, &buffer, &size);
	if(ret < OK) {
		return ret;
	}

	node = tpl_map("sf", string, value);
	if(tpl_load(node, TPL_MEM | TPL_UFREE, buffer, size) == -1) {
		printf("TPLR ERROR\n");
		return ERROR_TPL;
	}
	tpl_unpack(node, 0);
	tpl_free(node);
	// TODO: Errors tpl

	return OK;
}

int write_s(connection_t connection, char * string) {
	void * buffer;
	int size, ret;

	if(tpl_jot(TPL_MEM, &buffer, &size, "s", &string) == -1) {
		printf("TPLS ERROR\n");
		return ERROR_TPL;
	}

	ret = send(connection, buffer, size);

	free(buffer);

	return ret;
}

int read_s(connection_t connection, char ** string) {
	tpl_node * node;
	void * buffer;
	int size, ret;

	ret = receive(connection, &buffer, &size);
	if(ret < OK) {
		return ret;
	}

	node = tpl_map("s", string);
	if(tpl_load(node, TPL_MEM | TPL_UFREE, buffer, size) == -1) {
		printf("TPLR ERROR\n");
		return ERROR_TPL;
	}
	tpl_unpack(node, 0);
	tpl_free(node);
	// TODO: Errors tpl

	return OK;
}

int write_d(connection_t connection, double value) {
	void * buffer;
	int size, ret;

	if(tpl_jot(TPL_MEM, &buffer, &size, "f", &value) == -1) {
		printf("TPLS ERROR\n");
		return ERROR_TPL;
	}

	ret = send(connection, buffer, size);

	free(buffer);

	return ret;
}

int read_d(connection_t connection, double * value) {
	tpl_node * node;
	void * buffer;
	int size, ret;

	ret = receive(connection, &buffer, &size);
	if(ret < OK) {
		return ret;
	}

	node = tpl_map("f", value);
	if(tpl_load(node, TPL_MEM | TPL_UFREE, buffer, size) == -1) {
		printf("TPLR ERROR\n");
		return ERROR_TPL;
	}
	tpl_unpack(node, 0);
	tpl_free(node);
	// TODO: Errors tpl

	return OK;
}

static int send(connection_t connection, void * buffer, int size) {
	int send;

	send = server_send(connection, &size, sizeof(int));
	if(send == -1) {
		printf("1) ERROR\n");
		return ERROR_SEND;
	}
	if(send != sizeof(int)) {
		printf("2) ERROR\n");
		return ERROR_CORRUPT;
	}

	send = server_send(connection, buffer, size);
	if(send == -1) {
		printf("3) ERROR\n");
		return ERROR_SEND;
	}
	if(send != size) {
		printf("4) ERROR\n");
		return ERROR_CORRUPT;
	}

	return OK;
}

static int receive(connection_t connection, void ** buffer, int * size) {
	int receive;

	if(server_receive(connection, size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return ERROR_RECEIVE;
	}
	// TODO: Error si no recibi sizeof(int)?

	*buffer = malloc(*size);
	if(*buffer == NULL) {
		printf("6) ERROR\n");
		return ERROR_MALLOC;
	}

	receive = server_receive(connection, *buffer, *size);
	if(receive == -1) {
		printf("7) ERROR\n");
		return ERROR_RECEIVE;
	}
	if(receive != *size) {
		printf("8) ERROR\n");
		return ERROR_CORRUPT;
	}

	return OK;
}
