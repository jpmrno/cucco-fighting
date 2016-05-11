#include <stdio.h>
#include "options.h"
#include "middle_client.h"

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
	printf("  ,---.\n");
	printf(" ' __O>`\n");
	printf("( (__/  )\n");
	printf(" .-----,\n");
	printf("  `---'\n");
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
		printf("YOU HAVE %d CUCCOPESOSS\n", money);
		
	}
}

void printCuccoList(){
	list_cucco_t* ret = list_cuccos();
	if(ret == NULL){
		printf("There has been an error\n");
		return;
	}
	printf("   ___                        __ _     _   \n");
	printf("  / __\\   _  ___ ___ ___     / /(_)___| |_ \n");
	printf(" / / | | | |/ __/ __/ _ \\   / / | / __| __|\n");
	printf("/ /__| |_| | (_| (_| (_) | / /__| \\__ \\ |_ \n");
	printf("\\____/\\__,_|\\___\\___\\___/  \\____/_|___/\\__|\n");
	char** clist = ret->cucco_list;
	int i,j;
	for(i=0; i < ret->count; i++){
		printf("Cucco Nr %d: %s\n", i+1, clist[i]);
	}
	//free(ret);
	
}
