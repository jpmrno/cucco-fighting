#include <slib.h>
#include <define.h>
#include <library.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdio.h> // TODO: Remove

int opcode(connection_t connection) { // TODO: Change to opcode_t
	int ret, value;

	assert(connection != NULL);

	ret = read_i(connection, &value);
	if(ret < OK) {
		return ret;
	}

	return value;
}

// int login(connection_t connection) {
// 	char * user;
// 	int ret, value;

// 	if(connection == NULL) {
// 		printf("USER ERROR\n");
// 		return ERROR_CONNECTION;
// 	}

// 	ret = read_s(connection, &user);
// 	if(ret < OK) {
// 		return ret;
// 	}

// 	printf("(USER) -> %s\n", user);

// 	// TODO:
// 	value = 75;

// 	ret = write_i(connection, value);
// 	if(ret < OK) {
// 		return ret;
// 	}

// 	return value;
// }

int money(connection_t connection, double money) {
	assert(connection != NULL);

	return !(write_d(connection, money) < OK);
}

int cucco_add(connection_t connection) {
	char * cucco;
	int value;

	assert(connection != NULL);

	if(read_s(connection, &cucco) < OK) {
		return FALSE;
	}

	// TODO:
	value = 95;

	return !(write_i(connection, value) < OK);
}

int cucco_remove(connection_t connection) {
	char * cucco;
	int value;

	assert(connection != NULL);

	if(read_s(connection, &cucco) < OK) {
		return FALSE;
	}

	// TODO:
	value = 95;

	return !(write_i(connection, value) < OK);
}

int list(connection_t connection) {
	assert(connection != NULL);

	// TODO:
	char * cuccos[5] = {"Cucco 1", "Cucco 2", "Cucco 3", "Cucco 4", "Cucco 5"};
	int length = 5;

	return !(write_sa(connection, cuccos, length) < OK);
}

int bet(connection_t connection, double * wallet, int * clients, int * bettors, char ** winner) {
	char * cucco;
	double money;
	double value;

	assert(connection != NULL);

	if(read_sf(connection, &cucco, &money) < OK) {
		return FALSE;
	}

	printf("(BET) -> %f to %s\n", money, cucco);

	if(money < 0) {
		value = 0;
	} else {
		if(money > *wallet) {
			money = *wallet;
		}

		if(*bettors == 0) {
			*winner = NULL;
		}

		(*bettors)++;
		if(*bettors == *clients) {
			printf("FIGHT INIT: %d/%d\n", *bettors, *clients);
			*winner = "asd";
			// TODO: figth();
		}
		printf("BETTORS: %d\n", *bettors);
		while(*winner == NULL);

		if(!strcmp(cucco, *winner)) {
			(*wallet) += money;
		} else {
			(*wallet) -= money;
		}
		(*bettors)--;
	}

	// TODO:
	value = 65;

	return !(write_i(connection, value) < OK);
}

int reset(connection_t connection) {
	int value;

	assert(connection != NULL);

	value = TRUE;

	return !(write_i(connection, value) < OK);
}

int logout(connection_t connection) {
	int value;

	assert(connection != NULL);

	value = TRUE;

	return !(write_i(connection, value) < OK);
}
