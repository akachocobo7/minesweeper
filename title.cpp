#include "title.hpp"
#include "game.hpp"
#include <ncurses.h>
#include <array>
#include <string.h>


void viewTitle(int height, int width, int mine_num);
void viewHelp();


void viewTitle(int height, int width, int mine_num){
    initscr(); // ncursesの初期化
    
    int y, x;
    getmaxyx(stdscr, y, x); // 端末の横幅と縦幅を取得
    
    keypad(stdscr, TRUE); // カーソルキーを利用
    
    int cur = 0; // カーソルの位置    
    for(;;){
        clear();
        
        char title_msg[] = "minesweeper";
        mvprintw(y / 4, x / 2 - strlen(title_msg), "%s", title_msg);
    
        std::array< char[6], 3 > menu_msg = {
            "Start",
            "help",
            "Exit"
        };
    
        for(int i = 0; i < menu_msg.size(); i++){
            mvprintw(y / 2 + i, x / 2 - 3, "%s", menu_msg[i]);
        }
        mvprintw(y / 2 + cur, x / 2 - 6, "->");
        
        refresh();
        
        noecho();
        
        int ch = getch();
        if(ch == KEY_UP){
            cur--;
            if(cur < 0) cur += menu_msg.size();
        }
        if(ch == KEY_DOWN){
            cur++;
            cur %= menu_msg.size();
        }
        if(ch == '\n'){
            if(cur == 0){
                mainGame(height, width, mine_num);
            }
            if(cur == 1){
                
            }
            if(cur == 2){
                break;
            }
        }
    }
    
    endwin(); // ncurses終了
}


void viewHelp(){
    
}