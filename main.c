#include "db_utils.h"

int main(int argc, char *argv[]) {
  sqlite3 *db;

  if (open_db("x.db", &db) != SQLITE_OK) {
      return 1;     
  }
  t_names tables = get_tables_name(db);  
  


  for(int i = 0 ; i<tables.len ; i++){
    printf("%s\n", tables.names[i]); 
  }

  free_t_names(&tables);
  sqlite3_close(db);
  return 0;
}
