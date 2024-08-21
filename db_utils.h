
// db_utils.h

#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **names ; 
  int len ;
}t_names ;



int open_db(char* name , sqlite3 **db) ;
static int tnames_callback(void* data ,int argc , char **argv, char **azColName) ;
t_names get_tables_name(sqlite3 *db);
void free_t_names(t_names *t) ;


#endif // DB_H
