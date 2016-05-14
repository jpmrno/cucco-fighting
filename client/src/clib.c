#include <clib.h>
#include <define.h>
#include <library.h>
#include <string.h>

#define CONFIG_PATH_DEFAULT "config.ini"

static int opcode(opcode_t op);

static int login(char * user);
static int logout();

static connection_t connection = NULL;
static char * config_path = CONFIG_PATH_DEFAULT;

int config(char * path) {
	if(connection != NULL || path == NULL) {
		return FALSE;
	}

	strcpy(config_path, path);
	return TRUE;
}

int connect(char * user) {
	if(connection != NULL) {
		return FALSE;
	}

	connection = server_connect(config_path);
	if(connection == NULL) {
		return FALSE;
	}

	// if(login(user)) {
	// 	server_disconnect(connection);
	// 	return FALSE;
	// }

	return TRUE;
}

void disconnect() {
	if(connection != NULL) {
		// logout();
		server_disconnect(connection);
	}
}

double money() {
	int ret;
	double value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(MONEY);
	if(ret < OK) {
		return ret;
	}

	ret = read_d(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int cucco_add(char * cucco) {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(CUCCO_ADD);
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

int cucco_remove(char * cucco) {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(CUCCO_REMOVE);
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

int list(char *** list, int * length) {
	int ret;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(LIST);
	if(ret < OK) {
		return ret;
	}

	return read_sa(connection, list, length);
}

int bet(char * cucco, double money) {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(BET);
	if(ret < OK) {
		return ret;
	}

	ret = write_sf(connection, cucco, money);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int reset() {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(RESET);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

static int opcode(opcode_t op) {
	return write_i(connection, op);
}

static int login(char * user) {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(USER);
	if(ret < OK) {
		return ret;
	}

	ret = write_s(connection, user);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

static int logout() {
	int ret, value;

	if(connection == NULL) {
		return ERROR_CONNECTION;
	}

	ret = opcode(EXIT);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}
