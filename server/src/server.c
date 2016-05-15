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

#define CONFIG_FILE_DEFAULT "config.ini"

#define MONEY_DEFAULT 50

static void handle_int(int sign);
static int handle(connection_t connection);
static int run(connection_t connection, int op);

static int * bettors;
static int * clients;
static char ** winner;

static connection_t connection = NULL;
static smemory_t database = NULL;

static double client_money = MONEY_DEFAULT;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int connection_numer = 0, ret, pid;
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

	if(!log_open()) {
		fprintf(stderr, "Can't connect logging server.\n");
		exit(EXIT_FAILURE);
	}

	// key = ftok("", 0); // TODO: ID define (0)
	// if(key == -1) {
	// 	fprintf(stderr, "Configuration file's path does not exist or cannot be accessed.\n");
	// 	exit(EXIT_FAILURE);
	// }

	bettors = mmap(NULL, sizeof(*bettors), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(bettors == MAP_FAILED) {
		log_send(LEVEL_ERROR, "[MAIN SV] Can't create neccessary data to operate.");
		exit(EXIT_FAILURE);
	}

	clients = mmap(NULL, sizeof(*clients), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(clients == MAP_FAILED) {
		log_send(LEVEL_ERROR, "[MAIN SV] Can't create neccessary data to operate.");
		exit(EXIT_FAILURE);
	}

	winner = mmap(NULL, sizeof(*winner), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(winner == MAP_FAILED) {
		log_send(LEVEL_ERROR, "[MAIN SV] Can't create neccessary data to operate.");
		exit(EXIT_FAILURE);
	}

	*bettors = 0;
	*clients = 0;
	*winner = NULL;

	database = smemory_open(8080); // TODO: define 8080
	if(database == NULL) {
		log_send(LEVEL_ERROR, "[MAIN SV] Can't reach the database.");
		exit(EXIT_FAILURE);
	}

	connection = server_open(config_file);
	if(connection == NULL) {
		log_send(LEVEL_ERROR, "[MAIN SV] Can't create the main connection.");
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
			(*clients)++;
			server_ajar(connection);

			ret = handle(connection_accepted);
			if(ret == EXIT_FAILURE) {
				log_send(LEVEL_ERROR, "[CHILD SV] There was an error handling client.");
			}

			server_close(connection_accepted);
			(*clients)--;
			exit(ret);
		}

		log_send(LEVEL_INFO, "[MAIN SV] Desconectando nueva conexion.");
		server_ajar(connection_accepted);
	}

	server_close(connection);
	smemory_close(database);
	log_close();

	return 0;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_close(connection);
		smemory_close(database);
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
				log_send(LEVEL_ERROR, "Error en el money.");
				return -1;
			}
		} break;

		case CUCCO_ADD: {
			if(!cucco_add(connection)) {
				log_send(LEVEL_ERROR, "Error en el add.");
				return -1;
			}
		} break;

		case CUCCO_REMOVE: {
			if(!cucco_remove(connection)) {
				log_send(LEVEL_ERROR, "Error en el remove.");
				return -1;
			}
		} break;

		case LIST: {
			if(list(connection)) {
				log_send(LEVEL_ERROR, "Error en el list.");
				return -1;
			}
		} break;

		case BET: {
			if(!bet(connection, &client_money, clients, bettors, winner)) {
				log_send(LEVEL_ERROR, "Error en el bet.");
				return -1;
			}
		} break;

		case RESET: {
			if(!reset(connection)) {
				log_send(LEVEL_ERROR, "Error en el reset.");
				return -1;
			}

			client_money = MONEY_DEFAULT;
		} break;

		case EXIT: {
			if(!logout(connection)) {
				log_send(LEVEL_ERROR, "Error en el exit.");
				return -1;
			}

			return 1;
		} break;

		default: {
			log_send(LEVEL_ERROR, "Invalid opcode.");
			return -1;
		} break;
	}

	return 0;
}
