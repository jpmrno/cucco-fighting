#include <database.h>
#include <define.h>
#include <sqlite3.h>
#include <stdio.h>

#define DATABASE_PATH_DEFAULT "database.sqlite"

#define SQL_DATABASE_INIT "CREATE TABLE IF NOT EXISTS Clients (" \
"	ID integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE DEFAULT(0)," \
"	Name varchar NOT NULL UNIQUE," \
"	Money integer NOT NULL DEFAULT(50)" \
");" \
"CREATE TABLE IF NOT EXISTS Cuccos (" \
"	ID integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE DEFAULT(0)," \
"	Name varchar NOT NULL UNIQUE" \
");"

typedef int (*callback_t)(void *, int, char **, char **);

static int database_sql(char * sql, callback_t callback);
static int callback(void * notUsed, int argc, char **argv, char ** azColName);

static char * database_path = DATABASE_PATH_DEFAULT;
static sqlite3 * database;

int database_open() {
	int ret = sqlite3_open(database_path, &database);

	if(ret != SQLITE_OK) {
		fprintf(stderr, "<db error> can't open database: %s\n", sqlite3_errmsg(database));
	}

	if(ret != SQLITE_OK || !database_sql(SQL_DATABASE_INIT, NULL)) {
		sqlite3_close(database);
		return FALSE;
	}

	return TRUE;
}

void database_close() {
	sqlite3_close(database);
}

int database_cuccos_add(char * name) {
	// http://stackoverflow.com/questions/2942370/how-to-insert-any-string-in-sqlite3-in-c
	char * sql = sqlite3_mprintf("INSERT OR IGNORE INTO Cuccos(Name) VALUES('%q')", name);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_get(char * name) {
	char * sql = sqlite3_mprintf("SELECT ID, Name FROM Cuccos WHERE Name = '%q'", name);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_getn(int id) {
	char * sql = sqlite3_mprintf("SELECT ID, Name FROM Cuccos WHERE ID = %d", id);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_remove(char * name) {
	char * sql = sqlite3_mprintf("DELETE FROM Cuccos WHERE Name = '%q'", name);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_removen(int id) {
	char * sql = sqlite3_mprintf("DELETE FROM Cuccos WHERE ID = %d", id);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_list() {
	int ret = database_sql("SELECT * FROM Cuccos", callback);

	return ret;
}

int database_clients_add(char * name) {
	// http://stackoverflow.com/questions/2942370/how-to-insert-any-string-in-sqlite3-in-c
	char * sql = sqlite3_mprintf("INSERT OR IGNORE INTO Clients(Name) VALUES('%q')", name);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_get(char * name) {
	char * sql = sqlite3_mprintf("SELECT ID, Name, Money FROM Clients WHERE Name = '%q'", name);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_getn(int id) {
	char * sql = sqlite3_mprintf("SELECT ID, Name, Money FROM Clients WHERE ID = %d", id);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_remove(char * name) {
	char * sql = sqlite3_mprintf("DELETE FROM Clients WHERE Name = '%q'", name);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_removen(int id) {
	char * sql = sqlite3_mprintf("DELETE FROM Clients WHERE ID = %d", id);
	int ret = database_sql(sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_list() {
	int ret = database_sql("SELECT * FROM Clients", callback);

	return ret;
}

int database_clients_money(char * name, int money) {
	char * sql = sqlite3_mprintf("UPDATE Clients SET Money = Money + %d WHERE NAME = '%q'", money, name);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_moneyn(int id, int money) {
	char * sql = sqlite3_mprintf("UPDATE Clients SET Money = Money + %d WHERE ID = %d", money, id);
	int ret = database_sql(sql, callback);

	sqlite3_free(sql);

	return ret;
}

static int database_sql(char * sql, callback_t callback) {
	char * error;

	if(sqlite3_exec(database, sql, callback, NULL, &error) != SQLITE_OK) {
		fprintf(stderr, "<db error> SQL: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}

	return TRUE;
}

// TODO: Cambiar a q lo guarde en una estructura
static int callback(void * notUsed, int argc, char **argv, char ** azColName) {
	int i;
	notUsed = NULL;

	for(i = 0; i < argc; i++) {
	    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}
