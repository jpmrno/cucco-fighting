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
	data->opcode = KACHING;
	data->dataStruct = (void*) username;
	return send_rcv();
}

char** list_cuccos(){
	data = malloc(sizeof(data_t));
	data->opcode = LIST;
	data->dataStruct = NULL;
	int rcv = send_rcv();
	if(rcv == -1){
		return NULL;
	}
	list_cucco_t list = malloc(sizeof(list_cucco_t));
	memcpy(list, data->dataStruct, sizeof(list_cucco_t));	
	free(data);
	char** listacucc = list->cucco_list;
	free(list);
	return listacucc;
}

int get_money(char* username){
	data = malloc(sizeof(data_t));
	data->opcode = WALLET;
	data->dataStruct = (void*) username;
	int rcv = send_rcv();
	if(rcv == -1)
		return -1;
	return *((int*)data->dataStruct);
}

int bet(char* cucco, double money, char* username){
	data = malloc(sizeof(data_t));
	bet_t bet = malloc(sizeof(bet_t));
	if( data == NULL || bet == NULL)
		return -1;
	bet.cucco_name = cucco;
	bet.bet = money;
	bet.username = username;
	data->opcode = BET;
	data->dataStruct = bet;
	return send_rcv();
}


