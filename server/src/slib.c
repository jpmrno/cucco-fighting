#include <slib.h>
#include <define.h>
#include <comapi.h>
#include <library.h>
#include <tpl.h>
#include <stdio.h>
#include <stdlib.h>

int login(connection_t connection) {
	tpl_node * node;
	void * buffer;
	int size, ret = 22;
	char * user;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	node = tpl_map("s", &user);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("User: %s\n", user);

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &ret) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	return ret;
}

int add_cucco(connection_t connection) {
	tpl_node * node;
	void * buffer;
	int size, ret = 1;
	char * cucco;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	node = tpl_map("s", &cucco);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Cucco: %s\n", user);

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &ret) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	return ret;
}

int remove_cucco(connection_t* connection){
	tpl_node * node;
	void * buffer;
	int size, ret = 1;
	char * cucco;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	node = tpl_map("s", &cucco);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	printf("Cucco: %s\n", user);

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &ret) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	return ret;
}

int bet(connection_t* connection){
	tpl_node * node;
	void * buffer;
	int size, ret = 1;
	char * cucco;
	float bet;

	if(connection == NULL) {
		printf("1) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, &size, sizeof(int)) == -1) {
		printf("2) ERROR\n");
		return FALSE;
	}

	buffer = malloc(size);
	if(buffer == NULL) {
		printf("3) ERROR\n");
		return FALSE;
	}

	if(server_receive(connection, buffer, size) == -1) {
		printf("4) ERROR\n");
		return FALSE;
	}

	node = tpl_map("s", &cucco);
	tpl_load(node, TPL_MEM, buffer, size);
	tpl_unpack(node, 0);
	tpl_free(node);

	if(server_receive(connection, &bet, sizeof(float)) == -1){
		
	}


	printf("Cucco: %s\n", user);

	if(tpl_jot(TPL_MEM, &buffer, &size, "i", &ret) == -1) {
		printf("5) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, &size, sizeof(int)) == -1) {
		printf("6) ERROR\n");
		return FALSE;
	}

	if(server_send(connection, buffer, size) == -1) {
		printf("7) ERROR\n");
		return FALSE;
	}

	return ret;
}


