#include <library.h>

connection_t * connection_new() {
	connection_t * connection = malloc(sizeof(connection_t));
	// TODO:
	// if(retval == NULL) {
	// 	return NULL;
	// }

	return connection;
}

void connection_free(connection_t * connection) {
	if(connection != NULL) {
		free(connection);
	}
}
