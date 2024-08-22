#include "db_utils.h"
#include <stdio.h>



int main(int argc, char *argv[]){
  
  sqlite3 *db ; 
  int rc = open_db("x.db" ,&db) ;
  if(rc != SQLITE_OK){
    fprintf(stderr , "error oppening the database\n") ;
  }

  char *table_name = "products "; 
  
  table_content *t = get_table_contents(db ,table_name);
  
  for(int i = 0 ; i<t->rows ; i++){
    for(int j = 0 ; j<t->cols ; j++){
      printf("|  %s  |" , t->content[i][j]); 
    }
    printf("\n") ;
  }
  
  return EXIT_SUCCESS;
}
