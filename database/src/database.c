#include <database.h>
#include <define.h>
#include <dblib.h>
#include <stdio.h>
#include <stdlib.h>
#include <smemory.h>
#include <signal.h>
#include <dbsvlib.h>
#include <tpl.h>

#define DATABASE_PATH_DEFAULT "database.sql"

static void handle_int(int sign);
static int handle(void * buffer, size_t size);

static int get(void * buffer, int size, int * op, char ** name);
static int db_ret(void ** buffer, size_t * size, int value);
static int callback(void * unused, int argc, char **argv, char ** azColName);

static database_t database = NULL;
static smemory_t server = NULL;

static void * resp_buffer;
static char * resp_aux;
static size_t resp_size;

int main(int argc, char const * argv[]) {
	const char * database_path;
	void * buffer;
	size_t size;
	int ret = 0;

	switch(argc) {
		case 1: {
			database_path = DATABASE_PATH_DEFAULT;
		} break;

		case 2: {
			database_path = argv[1];
		} break;

		default: {
			fprintf(stderr, "Usage: 'database.app [database_path]'.\n");
			exit(EXIT_FAILURE);
		}
	}

	database = database_open(database_path);
	if(database == NULL) {
		fprintf(stderr, "Can't open database.\n");
		exit(EXIT_FAILURE);
	}

	server = smemory_make(8080); // TODO: define 8080
	if(server == NULL) {
		fprintf(stderr, "Can't create the main connection.\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	while(!ret) {
		smemory_read(server, &buffer, &size);
		ret = handle(buffer, size);
	}

	smemory_remove(server);
	database_close(database);

	return ret == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		smemory_remove(server);
		database_close(database);
	}
}

static int handle(void * buffer, size_t size) {
	tpl_node * node;
	int op, ret;
	char * cucco;

	if(!get(buffer, size, &op, &cucco)) {
		return -1;
	}

	switch(op) {
		case DB_GET: {
			node = tpl_map("A(s)", &resp_aux);
			ret = database_cuccos_get(database, cucco, callback, node);
			tpl_dump(node, TPL_MEM, &resp_buffer, &resp_size);
			tpl_free(node);
		} break;

		case DB_ADD: {
			ret = database_cuccos_add(database, cucco);
			db_ret(&resp_buffer, &resp_size, ret);
		} break;

		case DB_REMOVE: {
			ret = database_cuccos_remove(database, cucco);
			db_ret(&resp_buffer, &resp_size, ret);
		} break;

		case DB_LIST: {
			node = tpl_map("A(s)", &resp_aux);
			ret = database_cuccos_list(database, callback, node);
			tpl_dump(node, TPL_MEM, &resp_buffer, &resp_size);
			tpl_free(node);
		} break;

		case DB_EXIT: {
			return 1;
		} break;

		default: {
			return -1;
		} break;
	}

	free(cucco);

	if(!smemory_write(server, resp_buffer, resp_size)) {
		free(resp_buffer);
		return -1;
	}

	free(resp_buffer);

	return ret ? 0 : -1;
}

static int get(void * buffer, int size, int * op, char ** name) {
	tpl_node * node;

	node = tpl_map("is", op, name);
	if(tpl_load(node, TPL_MEM, buffer, size) == -1) {
		return FALSE;
	}
	tpl_unpack(node, 0);
	tpl_free(node);

	return TRUE;
}

static int db_ret(void ** buffer, size_t * size, int value) {
	if(tpl_jot(TPL_MEM, buffer, size, "i", &value) == -1) {
		return FALSE;
	}

    return TRUE;
}

static int callback(void * tn, int argc, char **argv, char ** azColName) {
	tpl_node * node = (tpl_node *) tn;

	resp_aux = argv[0];
	tpl_pack(node, 1);

	return 0;
}
