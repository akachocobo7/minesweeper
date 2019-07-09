#include "game.hpp"
#include <ncurses.h>
#include <unistd.h>

void mainGame(const int height, const int width, const int mine_num){
    MineSweeper game(height, width, mine_num);
    
    game.initGame();
    
    clear();
    
    int cur_y = 0, cur_x = 0; // 入力時のカーソル位置
    
    for(;;){
        GameData data = game.getGameData();
        viewGame(data, height, width);
        if(data.is_gameover || data.is_gameclear){
            sleep(3); // ゲーム クリア（オーバー）画面を3秒表示してタイトルへ
            break;
        }
        const auto input = inputProcess(height, width, cur_y, cur_x);
        const int op = input.op, y = input.y, x = input.x;
        if(op == 1){
            if(game.openCell(y, x) == -1){
                mvprintw(2, 1, "flag placed.\n");
            }
        }
        else{
            game.putFlag(y, x);
        }
    }
}

inputData inputProcess(const int height, const int width, int &cur_y, int &cur_x){
    int screen_y, screen_x;
    getmaxyx(stdscr, screen_y, screen_x); // 端末の横幅と縦幅を取得
    const int begin_y = screen_y / 2 - height / 2 - 1;
    const int begin_x = screen_x / 2 - width * 3 / 2 - 1 - 3;
    
    curs_set(1); // カーソルを表示
    
    for(;;){
        move(begin_y + cur_y + 2, begin_x + (cur_x + 2) * 3);
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
    int screen_y, screen_x;
    getmaxyx(stdscr, screen_y, screen_x); // 端末の横幅と縦幅を取得
    const int begin_y = screen_y / 2 - height / 2 - 1;
    const int begin_x = screen_x / 2 - width * 3 / 2 - 1 - 3;
    
    move(begin_y, begin_x + 4);
    for(int x = 0; x < width; x++){
        printw("%3d", x + 1);
    }
    
    for(int y = 0; y < height; y++){
        move(begin_y + 2 + y, begin_x);
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
    if(data.is_gameover){
        mvprintw(height + 8, 10, "GAME OVER");
    }
    if(data.is_gameclear){
        mvprintw(height + 8, 8, "GAME CLEAR!!!");
    }
    refresh();
}