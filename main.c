#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "db_utils.h"

/*WINDOW *create_window(WINDOW *win ,int height , int width , int x , int y ){*/
/*  win = newwin() */
/*}*/

void draw_table(WINDOW *win,table_content *tc ,int *mxcols ){
  wmove(win , 1 , 1)  ;
  for (int r = 0; r < tc->rows; r++) {
    int current_x = 1;
    for (int c = 0; c < tc->cols; c++) {
        mvwprintw(win, r + 2, current_x, "%-*s", mxcols[c], tc->content[r][c]);
        current_x += mxcols[c] + 2;
        mvwprintw(win, r + 2, current_x - 1, "|");
    }
  }
  wrefresh(win);

}

void clear_content(WINDOW *win){
  wclear(win) ;
  box(win ,0 , 0); 
  wrefresh(win);
}

int *max_cols(table_content *t){
  int *x = malloc(sizeof(int )*t->cols);
  for(int i = 0 ; i<t->cols ; i++){
    int max = strlen(t->content[0][i]) ; 
    for(int j = 0 ; j<t->rows ; j++){
      int y = strlen(t->content[j][i]); 
      if (max < y) {
        max = y ;
      }
    }
    x[i] = max ;
  }

  return x;
}

int main(int argc, char *argv[]) {
  sqlite3 *db;

  if (open_db("x.db", &db) != SQLITE_OK) {
      return 1;     
  }
  t_names tables = get_tables_name(db);  
  table_content *tc = get_table_contents(db , tables.names[0]) ;
  int *mxcols = max_cols(tc) ;
  initscr() ; 
  noecho() ; 
  start_color() ; 
  keypad(stdscr , true);
  int maxX , maxY ; getmaxyx(stdscr , maxY , maxX) ;  
 



  box(stdscr , 0 ,0);
  
  refresh();
  
  // creating a a WINDOW for the table _ names ;
  WINDOW *t_names_win = newwin(maxY-2,20 ,1 , 1 );
  box(t_names_win , 0, 0); 
  int tx =1 , ty = 1  ;

  // creating a window for the table contetn ; 
  WINDOW *t_content = newwin(maxY-2 , maxX - 23 ,1 ,21); 
  box(t_content , 0 , 0) ; 
  wrefresh(t_content) ; 
  init_pair(1, COLOR_CYAN, COLOR_CYAN);

 //////////////////////////////////////// 
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  int row = 1 ; 
  wmove(t_names_win , 1 , 1); 
  for(int i = 0 ; i<tables.len ; i++){
   mvwprintw(t_names_win, row++, 1, "%s", tables.names[i]);
  }
  wrefresh(t_names_win); 

  
  
 int cursor_position = 2; 
  move(cursor_position,2 ); 
  for(;;){
  
     
    clear_content(t_content);
    tc = get_table_contents(db , tables.names[cursor_position-2])  ;
    mxcols = max_cols(tc) ;   
    wattron(t_names_win , COLOR_PAIR(1));   
    wprintw(t_names_win,tables.names[cursor_position-2]);
    wattroff(t_names_win ,COLOR_PAIR(1));
    
    draw_table(t_content ,tc ,mxcols); 
    wmove(stdscr ,cursor_position , 2 ); 
    int x = getch();

    mvwprintw(t_names_win ,cursor_position, 2, tables.names[cursor_position-2]);
    
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
     
    move(cursor_position , 2); 
    refresh(); 
  };
  clear();
  endwin();  
  free_t_names(&tables);
  sqlite3_close(db);
  return 0;
}
