#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const * argv[]) {
	connection_t connection;

	connection = c_connect("/tmp/server");
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	sleep(10);
	c_disconnect(connection);

	return 0;
}
