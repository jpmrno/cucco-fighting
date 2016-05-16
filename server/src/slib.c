#include <slib.h>
#include <define.h>
#include <library.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <tpl.h>
#include <stdlib.h>
#include <dbsvlib.h>
#include <random.h>
#include <semaphore.h>
#include <server.h> // TODO: Remove
#include <stdio.h> // TODO: Remove

static int db_opcode(void ** buffer, size_t * size, int op, char * name);
static int db_return(void * buffer, size_t size, int * value);
static int db_cuccos(void * buffer, size_t size, char *** cuccos, int * length);

static int list_request(char *** cuccos, int * length, smemory_t database);
static char * fight(smemory_t database);

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

int cucco_add(connection_t connection, smemory_t database) {
	char * cucco;
	void * buffer;
	size_t size;
	int ret;

	assert(connection != NULL);
	assert(database != NULL);

	if(read_s(connection, &cucco) < OK) {
		return FALSE;
	}

	if(!db_opcode(&buffer, &size, DB_ADD, cucco)) {
		free(cucco);
		return FALSE;
	}

	if(!smemory_write(database, buffer, size)) {
		free(cucco);
		free(buffer);
		return FALSE;
	}

	free(cucco);
	free(buffer);

	if(!smemory_read(database, &buffer, &size)) {
		return FALSE;
	}

	if(!db_return(buffer, size, &ret)) {
		free(buffer);
		return FALSE;
	}

	free(buffer);

	return !(write_i(connection, ret) < OK);
}

int cucco_remove(connection_t connection, smemory_t database) {
	char * cucco;
	void * buffer;
	size_t size;
	int ret;

	assert(connection != NULL);
	assert(database != NULL);

	if(read_s(connection, &cucco) < OK) {
		return FALSE;
	}

	if(!db_opcode(&buffer, &size, DB_REMOVE, cucco)) {
		free(cucco);
		return FALSE;
	}

	if(!smemory_write(database, buffer, size)) {
		free(cucco);
		free(buffer);
		return FALSE;
	}

	free(cucco);
	free(buffer);

	if(!smemory_read(database, &buffer, &size)) {
		return FALSE;
	}

	if(!db_return(buffer, size, &ret)) {
		free(buffer);
		return FALSE;
	}

	free(buffer);

	return !(write_i(connection, ret) < OK);
}

int list(connection_t connection, smemory_t database) {
	char ** cuccos;
	int ret, length, i;

	assert(connection != NULL);
	assert(database != NULL);

	if(!list_request(&cuccos, &length, database)) {
		return FALSE;
	}

	ret = write_sa(connection, cuccos, length);

	for(i = 0; i < length; i++) {
		free(cuccos[i]);
	}
	free(cuccos);

	return !(ret < OK);
}

int bet(connection_t connection, double * wallet, int * clients, int * bettors, char ** winner, smemory_t database) {
	char * cucco;
	double money;
	int ret;

	assert(connection != NULL);

	if(read_sf(connection, &cucco, &money) < OK) {
		return FALSE;
	}

	printf("(BET) -> %f to %s\n", money, cucco);

	if(money > *wallet) {
		money = *wallet;
	}

	if(*bettors == 0) {
		*winner = NULL;
	}

	sem_lock(server_sems, 0);
	(*bettors)++;
	sem_unlock(server_sems, 0);
	printf("BETTORS: %d/%d\n", *bettors, *clients);
	if(*bettors == *clients) {
		printf("FIGHT INIT: %d/%d\n", *bettors, *clients);
		*winner = fight(database);
		if(*winner == NULL) {
			return FALSE;
		}
		sem_unlock(server_sems, 2); // Libero al resto de los clientes
	} else {
		printf("(%d) PIDO EL LOCK\n", 2);
		ret = sem_lock(server_sems, 2); // Espero al resto de los clientes
		printf("(%d) TENGO EL LOCK\n", 2);
	}

	printf("(%d) PIDO EL LOCK\n", 0);
	sem_lock(server_sems, 0);
	printf("(%d) TENGO EL LOCK\n", 0);
	(*bettors)--;
	sem_unlock(server_sems, 0);

	printf("(%d) PIDO EL LOCK\n", 3);
	printf("(%d) TENGO EL LOCK\n", 3);
	if(!strcmp(cucco, *winner)) {
		(*wallet) = (*wallet) + money;
	} else {
		(*wallet) = (*wallet) - money;
	}

	ret = !(write_s(connection, *winner) < OK);
	printf("TERMINE\n");

	return ret;
}

int reset(connection_t connection) {
	int value;

	assert(connection != NULL);

	value = TRUE;

	return !(write_i(connection, value) < OK);
}

int db_close(smemory_t database) {
	void * buffer;
	size_t size;

	if(!db_opcode(&buffer, &size, DB_EXIT, NULL)) {
		return FALSE;
	}

	if(!smemory_write(database, buffer, size)) {
		free(buffer);
		return FALSE;
	}

	free(buffer);

	return smemory_close(database);
}

static int db_opcode(void ** buffer, size_t * size, int op, char * name) {
	if(tpl_jot(TPL_MEM, buffer, size, "is", &op, &name) == -1) {
		return FALSE;
	}

	return TRUE;
}

static int db_return(void * buffer, size_t size, int * value) {
	tpl_node * node;

	node = tpl_map("i", value);
	if(tpl_load(node, TPL_MEM, buffer, size) == -1) {
		return FALSE;
	}
	tpl_unpack(node, 0);
	tpl_free(node);

	return TRUE;
}

static int db_cuccos(void * buffer, size_t size, char *** cuccos, int * length) {
	tpl_node * node;
	char * s;
	int i = 0;

	node = tpl_map("A(s)", &s);
	if(tpl_load(node, TPL_MEM | TPL_UFREE, buffer, size) == -1) {
		return FALSE;
	}
	*length = tpl_Alen(node, 1);
	*cuccos = malloc(sizeof(char*) * (*length));
	if(*cuccos == NULL) {
		return FALSE;
	}
	while(tpl_unpack(node,1) > 0) {
		(*cuccos)[i++] = s;
	}
	tpl_free(node);

	return TRUE;
}

static int list_request(char *** cuccos, int * length, smemory_t database) {
	void * buffer;
	size_t size;

	if(!db_opcode(&buffer, &size, DB_LIST, NULL)) {
		return FALSE;
	}

	if(!smemory_write(database, buffer, size)) {
		free(buffer);
		return FALSE;
	}

	free(buffer);

	if(!smemory_read(database, &buffer, &size)) {
		return FALSE;
	}

	if(!db_cuccos(buffer, size, cuccos, length)) {
		free(buffer);
		return FALSE;
	}

	return TRUE;
}

static char * fight(smemory_t database) {
	char ** cuccos, * cucco;
	int length, i_winner, i;

	if(!list_request(&cuccos, &length, database)) {
		return NULL;
	}

	i_winner = pcg32_boundedrand(length);
	cucco = cuccos[i_winner];

	for (i = 0; i < length; i++) {
		if(i != i_winner) {
			free(cuccos[i]);
		}
	}
	free(cuccos);

	return cucco;
}
