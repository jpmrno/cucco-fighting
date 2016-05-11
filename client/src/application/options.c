#include <stdio.h>
#include "options.h"





void kaching(char* username){
	int aux = reset_money(username);
	
	if(aux == -1){
		printf("An error was produced\n");
		return;
	}
	
	printf(" _ _  _,  _, _,_ _ _, _  _, ,\n");
	printf(" |_/ /_\\ / ` |_| | |\\ | / _ |\n");
	printf(" | \\ | | \\ , | | | | \\| \\ / |\n");
	printf(" ~ ~ ~ ~  ~  ~ ~ ~ ~  ~  ~  .\n");
	printf("\nYou now have 50 cuccope$so$!\n");
}

void getMoney(char* username){
	int money = get_money(username);
	if(money == -1){
		printf("An error has ocurred");
	}else{
		printf("  ,---.\n");
		printf(" ' __O>`\n");
		printf("( (__/  )\n");
		printf(" .-----,\n");
		printf("  `---'\n");
		printf("YOU HAVE 50 CUCCOPESOSS\n");
		
	}
}
