#include "db_utils.h"
#include <stdio.h>
#include <string.h>





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
      for (int i = 0; i < t.len; i++) {
          free(t.names[i]);
      }
      free(t.names);
      t.names = NULL; 
      t.len = 0; 
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
};

static int table_contents_callback(void* data , int argc , char **argv , char **azColName){
  static int rowCount = 0 ; 
  table_content *thing = (table_content *)data ; 
  thing->cols = argc ;   
  
  thing->content = realloc(thing->content ,(thing->rows +1)*sizeof(char **)) ; 

  char **col = malloc(argc * sizeof(char *));
  for(int i = 0 ; i<argc ; i++){
    col[i] = malloc( strlen(argv[i])*sizeof(char)  ) ;
    strcpy(col[i] , argv[i]) ;
  }
  thing->content[thing->rows] = col ;

  thing->rows++ ; 

  return 0 ;
}

table_content *get_table_contents(sqlite3 *db , char *table_name){
  char sql[200] = "SELECT * FROM  " ;
  strcat(sql ,table_name);
  strcat(sql , " ;") ;
  printf("%s\n" , sql);
  char *zErrMsg = 0;
  
  table_content *thing = malloc(sizeof(table_content)); 
  thing->rows = 0;
  thing->cols = 0;
  thing->content = NULL; 
  int rc = sqlite3_exec(db ,sql ,table_contents_callback ,(void *)thing , &zErrMsg );
  if(rc != SQLITE_OK){
    fprintf(stderr , "error executing the get contents query \n") ;  
  }

  return thing ;
}

