#ifndef _COMMON_LIB_H_
#define _COMMON_LIB_H_

#define MAX_NAME 20

// ---[ General ]------------------------------------
typedef enum {
	USER = 0, MONEY, CUCCO_ADD, CUCCO_REMOVE, LIST, BET, RESET, EXIT
} opcode_t;

typedef struct {
	opcode_t opcode;
	void * data;
} request_t;

typedef struct {
	void * data;
} response_t;
// --------------------------------------------------

// ---[ Request ]------------------------------------
typedef struct {
	char * name;
	// char name[MAX_NAME];
} user_t;

typedef struct {
	// char * name;
	char name[MAX_NAME];
} cucco_t;

typedef struct {
	// char * cucco;
	char cucco[MAX_NAME];
	double money;
} bet_t;
// --------------------------------------------------

// ---[ Response ]-----------------------------------
typedef struct {
	int value;
} return_t;

typedef struct {
	double value;
} money_t;

// typedef struct {
// 	// char * name;
// 	char name[MAX_NAME];
// } cucco_t;

typedef struct {
	int size;
	cucco_t * cuccos;
	// cucco_t cuccos[MAX_CUCCOS];
} cuccos_t;
// --------------------------------------------------

#endif
