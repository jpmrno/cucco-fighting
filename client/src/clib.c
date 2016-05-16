#include <clib.h>
#include <define.h>
#include <library.h>
#include <assert.h>

static int opcode(connection_t connection, opcode_t op);

// static connection_t connection = NULL;

// int connect(char * user, char * config_path) {
// 	connection = server_connect(config_file);
// 	if(connection == NULL) {
// 		return FALSE;
// 	}

// 	if(!login(user)) {
// 		server_disconnect(connection);
// 		return FALSE;
// 	}

// 	return TRUE;
// }

// void disconnect() {
// 	server_disconnect(connection);
// }

// int login(connection_t connection, char * user) {
// 	int ret, value;

// 	assert(connection != NULL);

// 	ret = opcode(connection, USER);
// 	if(ret < OK) {
// 		return ret;
// 	}

// 	ret = write_s(connection, user);
// 	if(ret < OK) {
// 		return ret;
// 	}

// 	ret = read_i(connection, &value);
// 	if(ret < OK) {
// 		return ret;
// 	}

// 	return value;
// }

double money(connection_t connection) {
	int ret;
	double value;

	assert(connection != NULL);

	ret = opcode(connection, MONEY);
	if(ret < OK) {
		return ret;
	}

	ret = read_d(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int cucco_add(connection_t connection, char * cucco) {
	int ret, value;

	assert(connection != NULL);

	ret = opcode(connection, CUCCO_ADD);
	if(ret < OK) {
		return ret;
	}

	ret = write_s(connection, cucco);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int cucco_remove(connection_t connection, char * cucco) {
	int ret, value;

	assert(connection != NULL);

	ret = opcode(connection, CUCCO_REMOVE);
	if(ret < OK) {
		return ret;
	}

	ret = write_s(connection, cucco);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int list(connection_t connection, char *** list, int * length) {
	int ret;

	assert(connection != NULL);

	ret = opcode(connection, LIST);
	if(ret < OK) {
		return ret;
	}

	return read_sa(connection, list, length);
}

char * bet(connection_t connection, char * cucco, double money) {
	int ret;
	char* winner;

	assert(connection != NULL);

	ret = opcode(connection, BET);
	if(ret < OK) {
		return NULL;
	}

	ret = write_sf(connection, cucco, money);
	if(ret < OK) {
		return NULL;
	}

	ret = read_s(connection, &winner);
	if(ret < OK) {
		return NULL;
	}

	return winner;
}

int reset(connection_t connection) {
	int ret, value;

	assert(connection != NULL);

	ret = opcode(connection, RESET);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int logout(connection_t connection) {
	assert(connection != NULL);

	return opcode(connection, EXIT) < OK ? FALSE : TRUE;
}

static int opcode(connection_t connection, opcode_t op) {
	return write_i(connection, op);
}
