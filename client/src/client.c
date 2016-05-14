#include <client.h>
#include <define.h>
#include <clib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static void handle_int(int sign);
void print_help();
void intro();
void signInOptions();
void menu();
void print_cuccos();
void print_money();
void reset_money();
void add_cucco();
void remove_cucco();
void place_bet();
void exit_cli();

static connection_t connection = NULL;

int main(int argc, char const * argv[]) {
	const char * config_file;

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
	
	int i=0;
	while(i<3){
		intro();
		i++;
	}
	signInOptions();
	menu();
	return 0;
}

void menu(){
	
	int alive = 1;
	char buffer[256];
	
	while(alive){
			printf("\nWhat would you like to do next?\n ");
			printf("enter 'help' to see the possible options:\n\n");
			bzero(buffer, 256);
			fgets(buffer, 255, stdin);

			if(strcmp(buffer,"list\n") == 0){
				print_cuccos();
				
			}else if (strcmp(buffer, "bet\n") == 0){
				place_bet();
				
			}else if(strcmp(buffer, "wallet\n") == 0){
				print_money();
				
			}else if(strcmp(buffer, "kaching\n") == 0){
				reset_money();
				
			}else if((strcmp(buffer, "help\n")) == 0 || (strcmp(buffer, "man\n")) == 0){
				print_help();	
				
			}else if(strcmp(buffer, "add\n")== 0){
				add_cucco();
			
			}else if(strcmp(buffer, "remove\n") == 0){
				remove_cucco();
				
			}else if(strcmp(buffer, "exit\n")== 0){
				alive = 0;
				exit_cli();
			}else{
					printf("Invalid command\n");
				}	
	  	  }
	  }


void print_help(){
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

void signInOptions(){

	printf("Enter your username, if it does not exist it will be created, if it does you will log into it\n\n");
	char user[30];
	bzero(user, 30);
	fgets(user, 29, stdin);
	int ret = login(connection, user);
	if(!ret) {
		fprintf(stderr, "No se pudo loguear al servidor.\n");
		server_disconnect(connection);
	}
}

void add_cucco(){
	printf("Type the name of the cucco you would like to add.\n");
	char cucco[30];
	bzero(cucco, 30);
	fgets(cucco, 29, stdin);
	printf("\n%s\n", cucco);
	
	int ret = cucco_add(connection, cucco);
	if(!ret) {
		fprintf(stderr, "No se pudo addear al servidor.\n");
		server_disconnect(connection);
	}
}

void remove_cucco(){
	printf("Type the name of the cucco you would like to remove.\n");
	char cucco[30];
	bzero(cucco, 30);
	fgets(cucco, 29, stdin);
	printf("\n%s\n", cucco);
	
	int ret = cucco_remove(connection, "La mas rapida");
	if(!ret) {
		fprintf(stderr, "No se pudo remove al servidor.\n");
		server_disconnect(connection);
	}
}

void print_cuccos(){
	int ret;
	char ** cuccos; // Remember to free! // TODO: Remove
	int length;
	ret = list(connection, &cuccos, &length);
	if(ret){
		fprintf(stderr, "No se pudo listear al servidor.\n");
		server_disconnect(connection);
	}
	int j;
	for(j=0; j<length; j++){
		printf("%s\n", cuccos[j]);
	}
}

void print_money(){
	int ret;
	ret = money(connection);
	printf("Money: %d\n", ret);
	if(!ret){
		fprintf(stderr, "No se pudo moneiar al servidor.\n");
		server_disconnect(connection);
	}	
}

void reset_money(){
	int ret = reset(connection);
	if(!ret){
		fprintf(stderr, "No se pudo resetear al servidor.\n");
		server_disconnect(connection);
	}else{
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
}

void place_bet(){
	printf("Enter the name of the cucco you would like to place a bet on.\n");
	char cucco[30];
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
	int ret = bet(connection, cucco, bett);
	if(!ret) {
		fprintf(stderr, "No se pudo bettear al servidor.\n");
		server_disconnect(connection);
	}
}

void exit_cli(){
	int ret = logout(connection);
	if(!ret) {
		fprintf(stderr, "No se pudo logoutear al servidor.\n");
		server_disconnect(connection);
	}
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}
}



void intro(){
	int i = 0, count = 0;
	
	while(i < 60){
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

