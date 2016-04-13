#include <database.h>
#include <define.h>
#include <sqlite3.h>
#include <stdio.h>

#define DATABASE_PATH_DEFAULT "database.sqlite"

#define SQL_DATABASE_INIT "CREATE TABLE IF NOT EXISTS Clients (" \
"	Number integer PRIMARY KEY AUTOINCREMENT NOT NULL DEFAULT(0)," \
"	Name varchar NOT NULL," \
"	Money integer NOT NULL DEFAULT(50)" \
");" \
"CREATE TABLE IF NOT EXISTS Cuccos (" \
"	Number integer PRIMARY KEY AUTOINCREMENT NOT NULL DEFAULT(0)," \
"	Name varchar NOT NULL" \
");"

typedef int (*callback_t)(void *, int, char **, char **);

static int database_sql(char * sql, callback_t callback);

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

static int database_sql(char * sql, callback_t callback) {
	char * error;

	if(sqlite3_exec(database, sql, callback, NULL, &error) != SQLITE_OK) {
		fprintf(stderr, "<db error> SQL: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}

	return TRUE;
}
