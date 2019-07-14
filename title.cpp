#include "title.hpp"
#include "game.hpp"
#include <ncurses.h>
#include <array>
#include <string.h>
#include <unistd.h>


void viewTitle(int height, int width, int mine_num);
void viewHelp();


void viewTitle(int height, int width, int mine_num){
    initscr(); // ncursesの初期化
    
    int y, x;
    getmaxyx(stdscr, y, x); // 端末の横幅と縦幅を取得
    
    keypad(stdscr, TRUE); // カーソルキーを利用
    
    int cur = 0; // カーソルの位置
    curs_set(0); //カーソルを非表示
    for(;;){
        clear(); // 画面をクリア
        
        char title_name[] = "minesweeper"; // タイトル名
        mvprintw(y / 4, x / 2 - strlen(title_name) / 2 - 2, "%s", title_name);
    
        // メニュー名
        std::array< char[6], 3 > menu_name = {
            "Start",
            "Help",
            "Exit"
        };
        for(int i = 0; i < menu_name.size(); i++){
            mvprintw(y / 2 + i, x / 2 - 3, "%s", menu_name[i]);
        }
        
        char help_msg[] = "move: ↑↓ key  select: z"; // ヘルプメッセージ
        mvprintw(y / 2 + menu_name.size() + 2, x / 2 - strlen(help_msg) / 2 - 2, "%s", help_msg);
        
        mvprintw(y / 2 + cur, x / 2 - 6, "->");
        
        refresh();
        
        noecho(); // 入力文字を非表示にする
        
        int ch = getch();
        if(ch == KEY_UP){
            cur--;
            // 上端より上に行ったら下端にする
            if(cur < 0) cur += menu_name.size();
        }
        if(ch == KEY_DOWN){
            cur++;
            // 下端より下に行ったら上端にする
            cur %= menu_name.size();
        }
        if(ch == 'z'){
            if(cur == 0){
                mainGame(height, width, mine_num);
            }
            if(cur == 1){
                viewHelp();
            }
            if(cur == 2){
                break;
            }
        }
    }
    
    endwin(); // ncurses終了
}


void viewHelp(){
    int y, x;
    getmaxyx(stdscr, y, x); // 端末の横幅と縦幅を取得
    
    clear();
    
    FILE *fp;
    if((fp = fopen("help.txt", "r")) == NULL){
        char error_msg[] = "There is no \"help.txt\".";
        mvprintw(y / 2, x / 2 - strlen(error_msg) / 2 - 1, "%s", error_msg);
        char help_msg[] = "Return to title with z key.";
        mvprintw(y / 2 + 3, x / 2 - strlen(help_msg) / 2 - 1, "%s", help_msg);
        refresh();
        int ch = getch();
        while(ch != 'z'){
            ch = getch();
        }
        return;
    }
    
    char help_title[] = "How to play Minesweeper";
    mvprintw(y / 4, x / 2 - strlen(help_title) / 2 - 1, "%s", help_title);
    char text[1024];
    int line = 0;
    while(fgets(text, 1024, fp) != NULL){
        mvprintw(y / 4 + 2 + line, x / 8, "%s", text);
        line++;
    }
    char help_msg[] = "Return to title with z key.";
    mvprintw(y / 4 + 2 + line + 1, x / 2 - strlen(help_msg) / 2 - 1, "%s", help_msg);
    
    int ch = getch();
    while(ch != 'z'){
        ch = getch();
    }
}