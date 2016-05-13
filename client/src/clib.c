#include <clib.h>
#include <define.h>
#include <library.h>
#include <stdio.h> // TODO: Remove

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

int login(connection_t connection, char * user) {
	int ret, value;

	if(connection == NULL) {
		printf("USER ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = opcode(connection, USER);
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

double money(connection_t connection) {
	int ret;
	double value;

	if(connection == NULL) {
		printf("MONEY ERROR\n");
		return ERROR_CONNECTION;
	}

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

	if(connection == NULL) {
		printf("CUCCO_ADD ERROR\n");
		return ERROR_CONNECTION;
	}

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

	if(connection == NULL) {
		printf("CUCCO_REMOVE ERROR\n");
		return ERROR_CONNECTION;
	}

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

int bet(connection_t connection, char * cucco, double money) {
	int ret, value;

	if(connection == NULL) {
		printf("BET ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = opcode(connection, BET);
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

int reset(connection_t connection) {
	int ret, value;

	if(connection == NULL) {
		printf("RESET ERROR\n");
		return ERROR_CONNECTION;
	}

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
	int ret, value;

	if(connection == NULL) {
		printf("EXIT ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = opcode(connection, EXIT);
	if(ret < OK) {
		return ret;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

static int opcode(connection_t connection, opcode_t op) {
	return write_i(connection, op);
}
