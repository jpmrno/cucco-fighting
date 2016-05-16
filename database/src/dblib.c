#include <dblib.h>
#include <define.h>
#include <sqlite3.h>
#include <assert.h>
#include <stdio.h> // TODO: Remove

typedef sqlite3 dbsql_t;

#define DATABASE_PATH_DEFAULT "database.sqlite"

#define SQL_DATABASE_INIT "CREATE TABLE IF NOT EXISTS Cuccos (" \
"	Name varchar PRIMARY KEY NOT NULL UNIQUE" \
");"

database_t database_open(const char * path) {
	dbsql_t * database;
	char * error;

	if(sqlite3_open(path, &database) != SQLITE_OK) {
		sqlite3_close(database);
		return NULL;
	}

	if(sqlite3_exec(database, SQL_DATABASE_INIT, NULL, NULL, &error) != SQLITE_OK) {
		printf("SQL ERROR: %s\n", error);
		sqlite3_free(error);
		sqlite3_close(database);
		return NULL;
	}

	return (database_t) database;
}

void database_close(database_t database) {
	dbsql_t * db = (dbsql_t *) database;

	if(db != NULL) {
		sqlite3_close(db);
	}
}

int database_cuccos_get(database_t database, char * name, callback_t callback, void * value) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql, * error;

	assert(db != NULL);

	sql = sqlite3_mprintf("SELECT ID, Name FROM Cuccos WHERE Name = '%q'", name);
	if(sqlite3_exec(db, sql, callback, value, &error) != SQLITE_OK) {
		printf("SQL ERROR: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}
	printf("Getting... %s\n", name);

	sqlite3_free(sql);

	return TRUE;
}

int database_cuccos_add(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql, * error;

	assert(db != NULL);

	// http://stackoverflow.com/questions/2942370/how-to-insert-any-string-in-sqlite3-in-c
	sql = sqlite3_mprintf("INSERT OR IGNORE INTO Cuccos(Name) VALUES('%q')", name);
	if(sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK) {
		printf("SQL ERROR: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}
	printf("Adding... %s\n", name);

	sqlite3_free(sql);

	return TRUE;
}

int database_cuccos_remove(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql, * error;

	assert(db != NULL);

	sql = sqlite3_mprintf("DELETE FROM Cuccos WHERE Name = '%q'", name);
	if(sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK) {
		printf("SQL ERROR: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}
	printf("Removing... %s\n", name);

	sqlite3_free(sql);

	return TRUE;
}

int database_cuccos_list(database_t database, callback_t callback, void * value) {
	dbsql_t * db = (dbsql_t *) database;
	char * error;

	assert(db != NULL);

	if(sqlite3_exec(db, "SELECT * FROM Cuccos", callback, value, &error) != SQLITE_OK) {
		printf("SQL ERROR: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}
	printf("Listing...\n");

	return TRUE;
}
