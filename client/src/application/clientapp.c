#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include "../../../comapi/include/comapi.h"
#include "options.h"
#include "../middle/middle_client.h"

void intro();
void signInOptions();
void menu();
void printHelp();

//VER DE PASARLO POR PARAMETRO SIEMPRE ESO NO LO HACEMOS BIEN
static char* username;

//lado pi
int main(void){
	
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
			printf("enter 'help' to see the possible options:\n");
			bzero(buffer, 256);
			fgets(buffer, 255, stdin);

			if(strcmp(buffer,"list\n") == 0){
				printf("cucko1cuccccccc\n");
				
			}else if (strcmp(buffer, "bet\n") == 0){
				//funcion apostar
				
			}else if(strcmp(buffer, "wallet\n") == 0){
				getMoney(username);
				
			}else if(strcmp(buffer, "kaching\n") == 0){
				kaching(username);
				
			}else if((strcmp(buffer, "help\n")) == 0 || (strcmp(buffer, "man\n")) == 0){
				printHelp();
				
			}else if(strcmp(buffer, "exit\n")== 0){
				alive = 0;
				//matar servidor
				
			}else{
				printf("Invalid command\n");
			}	
	  }
}


void printHelp(){
	printf(" _         _   \n");
	printf("| |_  ___ | | __\n");
	printf("| . |/ ._>| || . \\\n");
	printf("|_|_|\\___.|_||  _/\n");
	printf("             |_|  \n\n");
	
	printf("-list: Shows the list of cuccos that are available for a fight.\n");
	printf("-bet: Bet on a specific cucco money.\n");
	printf("-wallet: Get amount of money you have.\n");
	printf("-kaching: Reset amount of money, to 50 cuccope$o$.\n");
	printf("-help: See this menu.\n");
	printf("-exit: Exit.\n");
}

void signInOptions(){

	printf("Enter the number of your desired option:\n");
	printf("1. Enter with an existing account\n");
	printf("2. Create a new account\n");

	char opt = '6';
	
	
	while(opt != '1' && opt !=  '2'){
	
	opt = getchar();
	
	char aux;
	while((aux=getchar()) != '\n'){
		if(aux != ' ')
			opt = '3';
	}
	
	switch(opt){
		case '1':
			printf("cuenta uqe existe \n");
			break;
		case '2':
			printf("nueva\n");
			break;
		default:
			printf("\nPlease enter a valid option\n");
		
		}
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
