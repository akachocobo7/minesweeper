#include "game.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <cstring>


void mainGame(const int height, const int width, const int mine_num){
    MineSweeper game(height, width, mine_num);
    
    game.initGame();
    
    clear();
    
    int cur_y = 0, cur_x = 0; // 入力時のカーソル位置
    
    // ゲーム中は常にループ
    for(;;){
        // ゲーム状況を取得
        GameData data = game.getGameData();
        // ゲーム画面を表示
        viewGame(data, height, width);
        
        // ゲームオーバー　もしくは　ゲームクリアしたらこのループから抜ける
        if(data.is_gameover || data.is_gameclear){
            for(;;){
                int ch = getch();
                // zキーでタイトルへ
                if(ch == 'z'){
                    break;
                }
            }
            break;
        }
        
        // 入力処理
        const auto input = inputProcess(height, width, cur_y, cur_x);
        const int op = input.op, y = input.y, x = input.x;
        
        clear();
        
        if(op == 1){
            // フラグが置いてある所を開けようとした時はエラーメッセージを出力
            if(game.openCell(y, x) == -1){
                int begin_y, begin_x;
                getBeginDrawCell(begin_y, begin_x, height, width);
                
                char msg[] = "flag placed.";
                mvprintw(begin_y + height + 1, begin_x + width / 2 * 3 - strlen(msg) / 2, "%s", msg);
            }
        }
        else{
            game.putFlag(y, x);
        }
    }
}

inputData inputProcess(const int height, const int width, int &cur_y, int &cur_x){
    int begin_y, begin_x;
    getBeginDrawCell(begin_y, begin_x, height, width);
    
    curs_set(1); // カーソルを表示
    
    for(;;){
        move(begin_y + cur_y, begin_x + cur_x * 3 + 2);
        int ch = getch();
        if(ch == KEY_UP){
            cur_y--;
            if(cur_y < 0) cur_y += height;
        }
        if(ch == KEY_DOWN){
            cur_y++;
            if(cur_y >= height) cur_y -= height;
        }
        if(ch == KEY_LEFT){
            cur_x--;
            if(cur_x < 0) cur_x += width;
        }
        if(ch == KEY_RIGHT){
            cur_x++;
            if(cur_x >= width) cur_x -= width;
        }
        if(ch == 'z'){
            curs_set(0); // カーソルを非表示
            return {1, cur_y, cur_x};
        }
        if(ch == 'x'){
            curs_set(0); // カーソルを非表示
            return {2, cur_y, cur_x};
        }
    }
}



void viewGame(GameData &data, const int height, const int width){
    int begin_y, begin_x;
    getBeginDrawCell(begin_y, begin_x, height, width);
    
    move(begin_y - 2, begin_x);
    for(int x = 0; x < width; x++){
        printw("%3d", x + 1);
    }
    
    for(int y = 0; y < height; y++){
        move(begin_y + y, begin_x - 4);
        printw("%3d ", y + 1);
        for(int x = 0; x < width; x++){
            if(data.is_flag_placed[y][x]){
                printw("  f");
            }
            else if(data.is_opened[y][x]){
                if(data.field[y][x] == MINE){
                    printw("  #");
                }
                else{
                    printw("%3d", data.field[y][x]);
                }
            }
            else{
                printw(" []");
            }
        }
    }
    
    char help_msg[] = "Return to title with z key.";
    if(data.is_gameover){
        char msg[] = "GAME OVER";
        mvprintw(begin_y + height + 1, begin_x + width / 2 * 3 - strlen(msg) / 2, "%s", msg);
        mvprintw(begin_y + height + 3, begin_x + width / 2 * 3 - strlen(help_msg) / 2, "%s", help_msg);
    }
    if(data.is_gameclear){
        char msg[] = "GAME CLEAR!!!";
        mvprintw(begin_y + height + 1, begin_x + width / 2 * 3 - strlen(msg) / 2, "%s", msg);
        mvprintw(begin_y + height + 3, begin_x + width / 2 * 3 - strlen(help_msg) / 2, "%s", help_msg);
    }
    refresh();
}

void getBeginDrawCell(int &begin_y, int &begin_x, int height, int width){
    int screen_y, screen_x;
    getmaxyx(stdscr, screen_y, screen_x); // 端末の横幅と縦幅を取得
    begin_y = screen_y / 2 - height / 2 + 1;
    begin_x = screen_x / 2 - width * 3 / 2 - 1;
}