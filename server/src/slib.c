#include <slib.h>
#include <define.h>
#include <library.h>
#include <stdio.h> // TODO: Remove

int opcode(connection_t connection) { // TODO: Change to opcode_t
	int ret, value;

	if(connection == NULL) {
		printf("OPCODE ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	printf("(OPCODE) -> %d\n", value);

	return value;
}

int login(connection_t connection) {
	char * user;
	int ret, value;

	if(connection == NULL) {
		printf("USER ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = read_s(connection, &user);
	if(ret < OK) {
		return ret;
	}

	printf("(USER) -> %s\n", user);

	// TODO:
	value = 0;

	ret = write_i(connection, value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int money(connection_t connection) {
	int ret;
	double value;

	if(connection == NULL) {
		printf("MONEY ERROR\n");
		return ERROR_CONNECTION;
	}

	printf("(MONEY)\n");

	// TODO:
	value = 12;

	ret = write_d(connection, value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int cucco_add(connection_t connection) {
	char * cucco;
	int ret, value;

	if(connection == NULL) {
		printf("CUCCO_ADD ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = read_s(connection, &cucco);
	if(ret < OK) {
		return ret;
	}

	printf("(ADD) -> %s\n", cucco);

	// TODO:
	value = 95;

	ret = write_i(connection, value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int cucco_remove(connection_t connection) {
	char * cucco;
	int ret, value;

	if(connection == NULL) {
		printf("CUCCO_REMOVE ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = read_s(connection, &cucco);
	if(ret < OK) {
		return ret;
	}

	printf("(REMOVE) -> %s\n", cucco);

	// TODO:
	value = 95;

	ret = write_i(connection, value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

int list(connection_t connection) {
	if(connection == NULL) {
		printf("LIST ERROR\n");
		return ERROR_CONNECTION;
	}

	printf("(LIST)\n");

	// TODO:
	char * cuccos[5] = {"Cucco 1", "Cucco 2", "Cucco 3", "Cucco 4", "Cucco 5"};
	int length = 3;

	return write_sa(connection, cuccos, length);
}

int bet(connection_t connection) {
	char * cucco;
	double money;
	int ret, value;

	if(connection == NULL) {
		printf("BET ERROR\n");
		return ERROR_CONNECTION;
	}

	ret = read_sf(connection, &cucco, &money);
	if(ret < OK) {
		return ret;
	}

	printf("(BET) -> %f to %s\n", money, cucco);

	// TODO:
	value = 65;

	ret = write_i(connection, value);
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

	printf("(RESET)\n");

	// TODO:
	value = 21;

	ret = write_i(connection, value);
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

	printf("(EXIT)\n");

	// TODO:
	value = 54;

	ret = write_i(connection, value);
	if(ret < OK) {
		return ret;
	}

	return value;
}
