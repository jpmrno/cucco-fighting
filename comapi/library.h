#ifndef _COMMON_LIB_H_
#define _COMMON_LIB_H_

#include <comapi.h>

#define MAX_NAME 20

#define OK 0
#define ERROR -1
#define ERROR_TPL -2
#define ERROR_SEND -3
#define ERROR_CORRUPT -4
#define ERROR_RECEIVE -5
#define ERROR_MALLOC -6
#define ERROR_CONNECTION -7

// ---[ General ]------------------------------------
typedef enum {
	USER = 0, MONEY, CUCCO_ADD, CUCCO_REMOVE, LIST, BET, RESET, EXIT
} opcode_t;

// typedef struct {
// 	opcode_t opcode;
// 	void * data;
// } request_t;

// typedef struct {
// 	void * data;
// } response_t;
// --------------------------------------------------

// ---[ Request ]------------------------------------
// typedef struct {
// 	char * name;
// 	// char name[MAX_NAME];
// } user_t;

// typedef struct {
// 	// char * name;
// 	char name[MAX_NAME];
// } cucco_t;

// typedef struct {
// 	// char * cucco;
// 	char cucco[MAX_NAME];
// 	double money;
// } bet_t;
// --------------------------------------------------

// ---[ Response ]-----------------------------------
// typedef struct {
// 	int value;
// } return_t;

// typedef struct {
// 	double value;
// } money_t;

// // typedef struct {
// // 	// char * name;
// // 	char name[MAX_NAME];
// // } cucco_t;

// typedef struct {
// 	int size;
// 	cucco_t * cuccos;
// 	// cucco_t cuccos[MAX_CUCCOS];
// } cuccos_t;
// --------------------------------------------------

int write_i(connection_t connection, int value);
int read_i(connection_t connection, int * value);

int write_sf(connection_t connection, char * string, double value);
int read_sf(connection_t connection, char ** string, double * value);

int write_s(connection_t connection, char * string);
int read_s(connection_t connection, char ** string);

int write_d(connection_t connection, double value);
int read_d(connection_t connection, double * value);

int write_sa(connection_t connection, char ** string, int length);
int read_sa(connection_t connection, char *** string, int * length);

#endif
