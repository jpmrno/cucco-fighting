#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../comapi/include/library.h"

static data_t* data;
//static connection_t connection;
static char* config_file;

int send_rcv(){
	connection_t c =  server_connect(config_file);
	int send = server_send(c, (void*)data, sizeof(data_t));
	if(send == -1)
		return -1;
	int rcv = server_recieve(c, (void*) data, sizeof(data_t));
	if(rcv == -1)
		return -1;
	server_disconnect(c);
	return 1;
}

int reset_money(char* username){
	data=malloc(sizeof(data_t));
	if(data == NULL)
		return -1;
	data->opcode = KACHING;
	user_t* user = malloc(sizeof(user_t));
	user->username = username;
	memcpy(data->dataStruct, user, sizeof(user_t));
	return send_rcv();
}

list_cucco_t* list_cuccos(){
	data = malloc(sizeof(data_t));
	if(data == NULL)
		return NULL;
	data->opcode = LIST;
	data->dataStruct = NULL;
	int rcv = send_rcv();
	if(rcv == -1){
		return NULL;
	}
	list_cucco_t* list = malloc(sizeof(list_cucco_t));
	if(list == NULL)
		return NULL;
	memcpy(list, data->dataStruct, sizeof(list_cucco_t));\
	free(data);
	return list;
}

int get_money(char* username){
	data = malloc(sizeof(data_t));
	data->opcode = WALLET;
	user_t* user = malloc(sizeof(user_t));
	if(user == NULL){
		return -1;
	}
	memcpy(data->dataStruct, user, sizeof(user_t));
	int rcv = send_rcv();
	if(rcv == -1)
		return -1;
	
	//revisar esto chicos aver si les parece que anda o flashie tomates
	return (money_t) data->dataStruct.money;
}

int bet(char* cucco, double money, char* username){
	data = malloc(sizeof(data_t));
	bet_t* bet = malloc(sizeof(bet_t));
	bet->cucco_name = cucco;
	bet->bet = money;
	bet->username = username;
	data->opcode = BET;
	memcpy(data->dataStruct, bet, sizeof(bet_t));
	return send_rcv();
}


