
// ---[ Data packages ]-----------------------------------

typedef enum{
	LIST = 0, BET, WALLET, KACHING, EXIT, LOGIN, NEW_ACC
}opcode_t;

typedef struct{
	opcode_t opcode;
	void* dataStruct;
}data_t;

// ----------------------------------------------------


// ---[ Requests ]-----------------------------------

typedef struct{
	char* cucco_name;
	char* username;
	double bet;
}bet_t;

typedef struct{
	char* username;
}user_t;

typedef struct{
	int count;
	char** cucco_list;
}list_cucco_t;

typedef struct{
	char* username;
	int money;
}money_t;




