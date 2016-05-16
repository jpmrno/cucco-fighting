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
#include <smemory.h>
#include <sys/mman.h>
#include <log.h>
#include <time.h>
#include <random.h>
#include <server.h>
#include <string.h>
#include <semaphores.h>

#define CONFIG_FILE_DEFAULT "config.ini"

#define MONEY_DEFAULT 50

#define SEM_SIZE 2

#define MAX_SIZE 50

static void handle_int(int sign);
static int handle(connection_t connection);
static int run(connection_t connection, int op);

static int * bettors;
static int * clients;
static char * winner;

static connection_t connection = NULL;
static smemory_t database = NULL;

static double client_money = MONEY_DEFAULT;

int server_sems = -1;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int connection_numer = 0, ret, pid;
	short vals[SEM_SIZE] = {-1, -1};
	// key_t key;

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

	pcg32_srandom(time(NULL), (intptr_t)&connection_numer);

	server_sems = sem_make(1234, SEM_SIZE, vals);
	if(server_sems == -1) {
		fprintf(stderr, "1 Can't create neccessary data to operate.\n");
		exit(EXIT_FAILURE);
	}
	printf("LOCK: %d\n", server_sems);

	bettors = mmap(NULL, sizeof(*bettors), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if(bettors == MAP_FAILED) {
		fprintf(stderr, "2 Can't create neccessary data to operate.\n");
		exit(EXIT_FAILURE);
	}

	clients = mmap(NULL, sizeof(*clients), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if(clients == MAP_FAILED) {
		fprintf(stderr, "3 Can't create neccessary data to operate.\n");
		exit(EXIT_FAILURE);
	}

	winner = mmap(NULL, sizeof(*winner) * MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if(winner == MAP_FAILED) {
		fprintf(stderr, "4 Can't create neccessary data to operate.\n");
		exit(EXIT_FAILURE);
	}

	*bettors = 0;
	*clients = 0;
	memset(winner, 0, sizeof(*winner) * MAX_SIZE);

	if(!log_open()) {
		fprintf(stderr, "Can't connect logging server.\n");
		exit(EXIT_FAILURE);
	}

	database = smemory_open(8080); // TODO: define 8080
	if(database == NULL) {
		fprintf(stderr, "Can't reach the database.\n");
		exit(EXIT_FAILURE);
	}

	connection = server_open(config_file);
	if(connection == NULL) {
		fprintf(stderr, "Can't create the main connection.\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);
	signal(SIGCHLD, handle_int);

	while(TRUE) {
		connection_t connection_accepted;

		connection_accepted = server_accept(connection);
		if(connection_accepted == NULL) {
			log_send(LEVEL_ERROR, "[MAIN SV] Can't connect to client.");
			exit(EXIT_FAILURE); // TODO: Exit?
		}

		connection_numer++;

		pid = fork();
		if(pid == -1) {
			log_send(LEVEL_ERROR, "[MAIN SV] Can't assign resources to client.");
			exit(EXIT_FAILURE); // TODO: Exit?
		}

		if(!pid) { // Child process
			sem_lock(server_sems, 1);
			(*clients)++;
			sem_unlock(server_sems, 1);
			server_ajar(connection);
			log_send(LEVEL_INFO, "[CHILD SV] Disconnecting main connection.");

			ret = handle(connection_accepted);
			if(ret == EXIT_FAILURE) {
				log_send(LEVEL_ERROR, "[CHILD SV] There was an error handling client.");
			}

			server_close(connection_accepted);
			log_send(LEVEL_INFO, "[CHILD SV] Closed client connection.");
			sem_lock(server_sems, 1);
			(*clients)--;
			sem_unlock(server_sems, 1);
			exit(ret);
		}

		log_send(LEVEL_INFO, "[MAIN SV] Disconnecting new connection.");
		server_ajar(connection_accepted);
	}

	server_close(connection);
	if(!db_close(database)) {
		log_send(LEVEL_ERROR, "[MAIN SV] Couldn't correctly logout from database.");
	}
	log_close();
	sem_remove(server_sems);

	return 0;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_close(connection);
		if(!db_close(database)) {
			log_send(LEVEL_ERROR, "[MAIN SV] Couldn't correctly logout from database.");
		}
		sem_remove(server_sems);
		log_close();
		exit(EXIT_FAILURE);
	} else if(sign == SIGCHLD) {
		int status;

		waitpid(-1, &status, 0);
	}
}

static int handle(connection_t connection) {
	int op, ret = 0;

	while(!ret) {
		op = opcode(connection);
		ret = run(connection, op);
	}

	return ret == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}

static int run(connection_t connection, int op) {
	switch(op) {
		case MONEY: {
			if(!money(connection, client_money)) {
				log_send(LEVEL_ERROR, "Error getting the money.");
				return -1;
			}else{
				log_send(LEVEL_INFO, "Succesfully got money.");
			}
		} break;

		case CUCCO_ADD: {
			if(!cucco_add(connection, database)) {
				log_send(LEVEL_ERROR, "Error adding the cucco.");
				return -1;
			}else{
				log_send(LEVEL_INFO, "Succesfully added cucco.");
			}
		} break;

		case CUCCO_REMOVE: {
			if(!cucco_remove(connection, database)) {
				log_send(LEVEL_ERROR, "Error removing the cucco.");
				return -1;
			}else{
				log_send(LEVEL_INFO, "Succesfully removed cucco.");
			}
		} break;

		case LIST: {
			if(!list(connection, database)) {
				log_send(LEVEL_ERROR, "Error getting the list of cuccos.");
				return -1;
			}else{
				log_send(LEVEL_INFO, "Sucesfully returned list of cuccos");
			}
		} break;

		case BET: {
			if(!bet(connection, &client_money, clients, bettors, winner, database)) {
				log_send(LEVEL_ERROR, "Error placing bet.");
				return -1;
			}else{
				log_send(LEVEL_INFO, "Succesfully placed bet.");
			}
		} break;

		case RESET: {
			if(!reset(connection)) {
				log_send(LEVEL_ERROR, "Error reseting the amount of money");
				return -1;
			}
			log_send(LEVEL_INFO, "Succesfully reseted the users amount of money");
			client_money = MONEY_DEFAULT;
		} break;

		case EXIT: {
			log_send(LEVEL_INFO, "[CHILD SV] Exiting.");
			return 1;
		} break;

		default: {
			log_send(LEVEL_ERROR, "Invalid opcode.");
			return -1;
		} break;
	}

	return 0;
}
