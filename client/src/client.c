#include <client.h>
#include <define.h>
#include <clib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <string.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static void handle_int(int sign);
static void help();
static void intro();
static void menu();
static void cuccos_list();
static void money_print();
static void money_reset();
static void cuccos_add();
static void cuccos_remove();
static void place_bet();
static void client_exit();

static connection_t connection = NULL;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int i = 0;

	switch(argc) {
		case 1: {
			config_file = CONFIG_FILE_DEFAULT;
		} break;

		case 2: {
			config_file = argv[1];
		} break;

		default: {
			fprintf(stderr, "Usage: 'server.app [config_file]'.\n");
			exit(EXIT_FAILURE);
		}
	}

	connection = server_connect(config_file);
	if(connection == NULL) {
		fprintf(stderr, "No se pudo conectar al servidor.\n");
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, handle_int);

	while(i++ < 3) { // TODO: Nati gato
		intro();
	}

	menu();
	return 0;
}

static void menu() {
	int alive = 1;
	char buffer[256];

	while(alive) {
		printf("\nWhat would you like to do next?\n ");
		printf("enter 'help' to see the possible options:\n\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		if(strcmp(buffer,"list\n") == 0){
			cuccos_list();

		}else if (strcmp(buffer, "bet\n") == 0){
			place_bet();

		}else if(strcmp(buffer, "wallet\n") == 0){
			money_print();

		}else if(strcmp(buffer, "kaching\n") == 0){
			money_reset();

		}else if((strcmp(buffer, "help\n")) == 0 || (strcmp(buffer, "man\n")) == 0){
			help();

		}else if(strcmp(buffer, "add\n")== 0){
			cuccos_add();

		}else if(strcmp(buffer, "remove\n") == 0){
			cuccos_remove();

		}else if(strcmp(buffer, "exit\n")== 0){
			alive = 0;
			client_exit();
		}else {
			printf("Invalid command\n");
		}
	}
}

static void help() {
	printf(" _         _   \n");
	printf("| |_  ___ | | __\n");
	printf("| . |/ ._>| || . \\\n");
	printf("|_|_|\\___.|_||  _/\n");
	printf("             |_|  \n\n");

	printf("-list: Shows the list of cuccos that are available for a fight.\n");
	printf("-add: Add a new cucco\n");
	printf("-remove: Remove a specific cucco.\n");
	printf("-bet: Bet on a specific cucco money.\n");
	printf("-wallet: Get amount of money you have.\n");
	printf("-kaching: Reset amount of money, to 50 cuccope$o$.\n");
	printf("-help: See this menu.\n");
	printf("-exit: Exit.\n");
}

static void cuccos_add() {
	char cucco[30];
	int ret;

	printf("Type the name of the cucco you would like to add:\n");
	bzero(cucco, 30);
	fgets(cucco, 29, stdin);

	ret = cucco_add(connection, cucco);
	if(ret < OK) { // TODO: Cambiar de lugar
		fprintf(stderr, "No se pudo cucco_add al servidor.\n");
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}

	if(ret) {
		printf("Cucco '%s' added successfuly.\n", cucco);
	} else {
		printf("Couldn't add cucco '%s'.\n", cucco);
	}
}

static void cuccos_remove() {
	char cucco[30];
	int ret;

	printf("Type the name of the cucco you would like to remove:\n");
	bzero(cucco, 30);
	fgets(cucco, 29, stdin);

	ret = cucco_remove(connection, cucco);
	if(ret < OK) { // TODO: Cambiar de lugar
		fprintf(stderr, "No se pudo cucco_remove al servidor.\n");
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}

	if(ret) {
		printf("Cucco '%s' removed successfuly.\n", cucco);
	} else {
		printf("Couldn't remove cucco '%s'.\n", cucco);
	}
}

static void cuccos_list() {
	char ** cuccos; // Remember to free! // TODO: Remove
	int length, i;

	if(list(connection, &cuccos, &length) < OK) { // TODO: Cambiar de lugar
		fprintf(stderr, "No se pudo listear al servidor.\n");
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < length; i++) {
		printf("%s\t", cuccos[i]);
		free(cuccos[i]);
	}
	free(cuccos);
}

static void money_print() {
	double wallet;

	wallet = money(connection);
	if(wallet < 0) { // TODO: Cambiar de lugar
		fprintf(stderr, "No se pudo monear al servidor.\n");
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}

	printf("You have: %f cuccope$o$\n", wallet);
}

static void money_reset() {
	if(reset(connection) < OK) { // TODO: Cambiar de lugar
		fprintf(stderr, "No se pudo resetear al servidor.\n");
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}

	printf(" _ _  _,  _, _,_ _ _, _  _, ,\n");
	printf(" |_/ /_\\ / ` |_| | |\\ | / _ |\n");
	printf(" | \\ | | \\ , | | | | \\| \\ / |\n");
	printf(" ~ ~ ~ ~  ~  ~ ~ ~ ~  ~  ~  .\n");
	printf("\t  ,---.\n");
	printf("\t ' __O>`\n");
	printf("\t( (__/  )\tYou now have 50 cuccope$o$!\n");
	printf("\t .-----,\n");
	printf("\t  `---'\n");
}

static void place_bet() {
	char * winner = NULL;
	char cucco[30];

	printf("Enter the name of the cucco you would like to place a bet on.\n");
	bzero(cucco, 30);
	fgets(cucco, 29, stdin);
	printf("\n%s\n", cucco);

	int aux = 1;
	char buff[30];
	printf("Enter the amount of money you would like to bet.\n");
	while(aux == 1){
		int flag = 0;
		int point = 0;
		fgets(buff, 29,stdin);
		int i;
		int length = strlen(buff);
		for(i=0; i<length; i++){
			if((buff[i] < '0' || buff[i] > '9') && buff[i]!='.' && buff[i]!='\n'){
				flag = 1;
			}if(buff[i] == '.'){
				if(point == 1){
					flag = 1;
				}
				point++;
			}
		}
		if(flag == 0){
			aux = 0;
		}else{
			printf("Please enter a valid bet\n");
		}
	}

	float bett= atof(buff);
	winner = bet(connection, cucco, bett);
	if(winner == NULL) {
		fprintf(stderr, "No se pudo bettear al servidor.\n");
		server_disconnect(connection);
	}

	printf("The winner is... %s\n", winner);
}

static void client_exit() {
	if(!logout(connection)) {
		fprintf(stderr, "Couldn't correctly logout from server.\n");
	}
	server_disconnect(connection);
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		client_exit();
		exit(EXIT_FAILURE);
	}
}

static void intro(){
	int i = 0, count = 0;

	while(i < 60) {
		system("clear");
		printf("    ___  __  __  ___  ___  _____    ____  ____  ___  _   _  ____ /\\\n");
		printf("   / __)(  )(  )/ __)/ __)(  _  )  ( ___)(_  _)/ __)( )_( )(_  _))(\n");
		printf("  ( (__  )(__)(( (__( (__  )(_)(    )__)  _)(_( (_-. ) _ (   )(  \\/\n");
		printf("   \\___)(______)\\___)\\___)(_____)  (__)  (____)\\___/(_) (_) (__) ()\n\n\n");

		for(count = 0; count < i; count++){
			printf(" ");
		}
		printf("      _   \n");

		for(count = 0; count < i; count++){
			printf(" ");
		}
		printf(" \\. _(9> \n");

		for(count = 0; count < i; count++){
			printf(" ");
		}
		printf("  \\==_) \n");

		for(count = 0; count < i; count++){
			printf(" ");
		}
		printf("   -'=   \n");

		printf("\n  \tBy: Juan Moreno, Francisco Bartolome, Natalia Navas\n\n");
		usleep(10500);
		i++;
	}
}
