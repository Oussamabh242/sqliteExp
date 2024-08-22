#include <ncurses.h>
#include <stdbool.h>
#include "db_utils.h"

int main(int argc, char *argv[]) {
  sqlite3 *db;

  if (open_db("x.db", &db) != SQLITE_OK) {
      return 1;     
  }
  t_names tables = get_tables_name(db);  
  


  /*for(int i = 0 ; i<tables.len ; i++){*/
  /*  printf("%s\n", tables.names[i]); */
  /*}*/

  initscr() ; 
  noecho() ; 
  start_color() ; 
  keypad(stdscr , true);


  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  int row = 2 ; 
  for(int i = 0 ; i<tables.len ; i++){
    mvprintw(row++ , 0 , tables.names[i]);
  }
  
  
 int cursor_position = 2; 
  move(cursor_position, 0); 
  for(;;){
  
       

    attron(COLOR_PAIR(1));   
    printw(tables.names[cursor_position-2]);
    attroff(COLOR_PAIR(1));
    
    int x = getch();
    
    mvprintw(cursor_position, 0, tables.names[cursor_position-2]);
    
    switch (x) {
      case KEY_UP:
        if (cursor_position > 2) {
          cursor_position--; 
        }
        break; 
      case KEY_DOWN:
        if (cursor_position <tables.len+1) {
          cursor_position++; 
        }  
        break ;
      case 'q':
        endwin() ; 
        return 0;
        break ; 
    }
     
    move(cursor_position , 0); 
    refresh(); 
  };
  clear();
  endwin();  
  free_t_names(&tables);
  sqlite3_close(db);
  return 0;
}
