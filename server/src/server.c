#include <server.h>
#include <slib.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <library.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <string.h>
#include <log.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static int handle(connection_t connection);
static void handle_int(int sign);
static int run(connection_t connection, int op);

static connection_t connection = NULL;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int connection_numer = 0;

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
	
	log_open();

	connection = server_open(config_file);
	if(connection == NULL) {
		log_send(LEVEL_ERROR, "Error in the connection");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);
	signal(SIGCHLD, handle_int);
	
	log_send(LEVEL_INFO, "HOLA");
	
	while(TRUE) {
		connection_t connection_accepted;
		int ret, pid;

		connection_accepted = server_accept(connection);
		if(connection_accepted == NULL) {
			log_send(LEVEL_ERROR, "Error in the new connection");
			exit(EXIT_FAILURE);
		}
		log_send(LEVEL_INFO, "A new connection has been added");
		connection_numer++;

		pid = fork();
		if(pid == -1) {
			log_send(LEVEL_ERROR, "Error with the fork");
			exit(EXIT_FAILURE);
		}

		if(!pid) { // Child process
			server_ajar(connection);

			ret = handle(connection_accepted);

			server_close(connection_accepted);
			exit(ret);
		}

		log_send(LEVEL_INFO, "Disconnecting new connection");
		server_ajar(connection_accepted);
	}

	log_send(LEVEL_INFO, "Disconnecting the main connection");
	log_close();
	server_close(connection);

	return 0;
}

static int handle(connection_t connection) {
	int op;

	while(TRUE) {
		op = opcode(connection);
		run(connection, op);
	}

	return TRUE;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_close(connection);
		exit(EXIT_FAILURE);
	} else if(sign == SIGCHLD) {
		int status;

		waitpid(-1, &status, 0);
	}
}

static int run(connection_t connection, int op) {
	switch(op) {
		case USER: {
			if(!login(connection)) {
				log_send(LEVEL_WARNING, "Error with the login");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully logged in");
			}
		} break;

		case MONEY: {
			if(!money(connection)) {
				log_send(LEVEL_WARNING, "Error getting the users current amount of money");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully getting the money");
			}
		} break;

		case CUCCO_ADD: {
			if(!cucco_add(connection)) {
				log_send(LEVEL_WARNING, "Error adding the cucco");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully adding the cucco");
			}
		} break;

		case CUCCO_REMOVE: {
			if(!cucco_remove(connection)) {
				log_send(LEVEL_WARNING, "Error removing the cucco");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully removed the cucco");
			}
		} break;

		case LIST: {
			if(list(connection)) {
				log_send(LEVEL_WARNING, "Error listing the cuccos");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully got the cucco list");
			}
		} break;

		case BET: {
			if(!bet(connection)) {
				log_send(LEVEL_WARNING, "Error with the bet");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully placed a bet");
			}
		} break;

		case RESET: {
			if(!reset(connection)) {
				log_send(LEVEL_WARNING, "Error reseting the users amount of money");
				return FALSE;
			}else{
				log_send(LEVEL_INFO, "Succesfully reset the users amount of money");
			}
		} break;

		case EXIT: {
			if(!logout(connection)) {
				log_send(LEVEL_WARNING, "Error exiting");
				return FALSE;
			}
			exit(0);
		} break;
	}

	return TRUE;
}
