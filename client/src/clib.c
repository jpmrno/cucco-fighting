#include <clib.h>
#include <define.h>
#include <library.h>
#include <tpl.h>
#include <stdlib.h>
#include <stdio.h>

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

static void opcode(int op);

static void opcode(opcode_t op) {
	void * buffer;
	int size;

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &op) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}
}

int login(connection_t connection, char * user) {
	tpl_node * node;
	void * buffer;
	int size, ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	opcode(LOGIN);
	
	if(tpl_jot(TPL_MEM, &buffer, &size, "s", &user) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("i", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
}

double money(connection_t connection) {
	tpl_node * node;
	void * buffer;
	int size;
	double ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	opcode(MONEY);

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("d", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
}

int bet(connection_t connection, char * cucco, double money){
	tpl_node * node;
	void * buffer;
	int size, ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}
	
	opcode(BET);

	if(tpl_jot(TPL_MEM, &buffer, &size, "sf", &cucco, &money) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("i", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
	
	
}



int cucco_add(connection_t connection, char * name){
	tpl_node * node;
	void * buffer;
	int size, ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}
	
	opcode(CUCCO_ADD);

	if(tpl_jot(TPL_MEM, &buffer, &size, "s", &cucco) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("i", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
}

int cucco_remove(connection_t connection, char * name){
	tpl_node * node;
	void * buffer;
	int size, ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}
	
	opcode(CUCCO_REMOVE);

	if(tpl_jot(TPL_MEM, &buffer, &size, "s", &cucco) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("i", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
}

int reset(connection_t connection){
	tpl_node * node;
	void * buffer;
	int size;
	double ret;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	opcode(RESET);

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	node = tpl_map("i", &ret);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Return: %d\n", ret);

	return ret;
}

