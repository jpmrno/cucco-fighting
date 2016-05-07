#include <logger.h>
#include <stdio.h>
#include <strings.h>

int main(int argc, char const * argv[]) {
	int ret;

	if(argc != 2) {
		return 1;
	}

	argv++;
	if(!strcpy(argv[0], "stdio")) {
		ret = logger_stdio();
	} else {
		ret = logger_file(argv[0]);
	}

	return !ret;
}

int logger_stdio() {

}

int logger_file(char * path) {
	FILE * file;

	file = fopen(path, "a+");
	if(file == NULL) {
		return FALSE;
	}
}
