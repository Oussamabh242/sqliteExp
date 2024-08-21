#include "db_utils.h"


static int rowCount = 0 ;



static int tnames_callback(void* data ,int argc , char **argv, char **azColName){
  t_names *tnames =(t_names *)data ;

  tnames->names = realloc(tnames->names, (tnames->len + 1) * sizeof(char *));
  if (tnames->names == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return 1; 
  }

  tnames->names[tnames->len] = strdup(argv[0]);
  tnames->len ++ ;
  return 0 ; 
  
};

int open_db(char *name , sqlite3 **db){
  int rc = sqlite3_open(name , db); 
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
    return rc;
  }
  fprintf(stderr, "Opened db successfully\n");
  return SQLITE_OK;
}

t_names get_tables_name(sqlite3 *db){
  char *sql =  "SELECT name FROM sqlite_master WHERE type='table';";
  t_names t ;
  t.len = 0; 
  t.names = NULL ;
  char *zErrMsg = 0;
  
  int rc = sqlite3_exec(db ,sql ,tnames_callback ,(void *)&t , &zErrMsg); 
  
  if (rc != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      // Free allocated memory before returning
      for (int i = 0; i < t.len; i++) {
          free(t.names[i]);
      }
      free(t.names);
      t.names = NULL; // Set to NULL to indicate failure
      t.len = 0; // Reset length
  }

  return t;  
}

void free_t_names(t_names *t){
  if (t != NULL){
    for(int i = 0 ; i<t->len ; i++){
      free(t->names[i]); 
    }
    free(t->names);
    t->names = NULL ;
  }
}
/*int execute_query(sqlite3 *db, const char *sql, t_names* data, int maxRows) {*/
/*    char *zErrMsg = 0;*/
/*    int rc = sqlite3_exec(db, sql, tnames_callback_callback, (void *)data, &zErrMsg);*/
/**/
/*    if (rc != SQLITE_OK) {*/
/*        fprintf(stderr, "SQL error: %s\n", zErrMsg);*/
/*        sqlite3_free(zErrMsg);*/
/*        return rc;*/
/*    } else {*/
/*        fprintf(stdout, "Operation done successfully\n");*/
/*    }*/
/**/
/*    return SQLITE_OK;*/
/*} */
