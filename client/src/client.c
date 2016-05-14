#include <client.h>
#include <define.h>
#include <clib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void handle_int(int sign);

int main(int argc, char * const argv[]) {
	char * config_path = NULL;
	char * user = "Juan Bartolome Navas"; // TODO: = NULL
	int c;

	opterr = 0;
	while((c = getopt(argc, argv, "u:c:")) != -1) {
		switch (c) {
			case 'u': {
				user = optarg;
			} break;

			case 'c': {
				config_path = optarg;
			} break;

			case '?': {
				if(optopt == 'u') {
					fprintf(stderr, "Option -%c requires a username.\n", optopt);
				} else if(optopt == 'c') {
					fprintf(stderr, "Option -%c requires configuration file's path.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				}
				exit(EXIT_FAILURE);
			} break;

			default: {
				abort();
			} break;
		}
	}

	if(argc - optind != 0) {
		fprintf(stderr, "Usage: 'client.app [-u 'username'] [-c 'config_path']'.\n");
		exit(EXIT_FAILURE);
	}

	if(user == NULL) {
		// TODO: Pedir usuario
	}

	config(config_path);

	if(!connect(user)) {
		fprintf(stderr, "No se pudo conectar al servidor.\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	printf("Bienvenido %s!\n", user);

	ret = money();
	if(!ret) {
		fprintf(stderr, "No se pudo moneiar al servidor.\n");
		disconnect();
	}

	ret = cucco_add("La mas rapida");
	if(!ret) {
		fprintf(stderr, "No se pudo addear al servidor.\n");
		disconnect();
	}

	ret = cucco_remove("La mas rapida");
	if(!ret) {
		fprintf(stderr, "No se pudo remove al servidor.\n");
		disconnect();
	}

	char ** cuccos; // Remember to free! // TODO: Remove
	int length; // TODO: Remove
	ret = list(&cuccos, &length);
	if(ret) {
		fprintf(stderr, "No se pudo listear al servidor.\n");
		disconnect();
	}
	int j; // TODO: Remove
	for(j=0;j<length;j++) { // TODO: Remove
		printf("%s\n", cuccos[j]);
	}

	ret = bet("La mas rapida", 20.4);
	if(!ret) {
		fprintf(stderr, "No se pudo bettear al servidor.\n");
		disconnect();
	}

	ret = reset();
	if(!ret) {
		fprintf(stderr, "No se pudo resetear al servidor.\n");
		disconnect();
	}

	disconnect();

	return 0;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		disconnect();
		exit(EXIT_FAILURE);
	}
}
