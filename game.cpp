#include "game.hpp"
#include <ncurses.h>
#include <unistd.h>

void mainGame(const int height, const int width, const int mine_num){
    MineSweeper game(height, width, mine_num);
    
    game.initGame();
    
    clear();
    echo(); // 入力文字を表示するようにする
    for(;;){
        GameData data = game.getGameData();
        viewGame(data, height, width);
        if(data.is_gameover || data.is_gameclear){
            sleep(3);
            break;
        }
        curs_set(1); // カーソルを表示
        const auto input = inputProcess(height, width);
        curs_set(0); // カーソルを非表示
        erase(); // 入力文字を消去
        const int op = input.op, y = input.y, x = input.x;
        if(input.op < 1 || input.op > 2 ||
           input.x < 1 || input.x > width ||
           input.y < 1 || input.y > height){
            // printw("入力が不正です。\n");
            mvprintw(2, 1, "out\n");
            continue;
        }
        if(op == 1){
            if(game.openCell(y - 1, x - 1) == -1){
                mvprintw(2, 1, "flag placed.\n");
            }
        }
        else{
            game.putFlag(y - 1, x - 1);
        }
    }
}

inputData inputProcess(const int height, const int width){
    for(;;){
        mvprintw(1, 1, "please input: ");
        char s[256] = {}; getstr(s);
        const auto input = split(s);
        return input;
    }
}

// 文字列を3つの数字に分割
inputData split(char s[]){
    char *p = s;
    
    int num1 = 0;
    while(*p && *p != ' '){
        if('0' <= *p && *p <= '9'){
            num1 *= 10;
            num1 += *p - '0';
        }
        else{
            return {-1, -1, -1};
        }
        p++;
    }
    while(*p == ' ') p++;
    
    int num2 = 0;
    while(*p && *p != ' '){
        if('0' <= *p && *p <= '9'){
            num2 *= 10;
            num2 += *p - '0';
        }
        else{
            return {-1, -1, -1};
        }
        p++;
    }
    while(*p == ' ') p++;
    
    int num3 = 0;
    while(*p && *p != ' '){
        if('0' <= *p && *p <= '9'){
            num3 *= 10;
            num3 += *p - '0';
        }
        else{
            return {-1, -1, -1};
        }
        p++;
    }
    while(*p == ' ' || *p == '\n') p++;
    
    if(!*p) return {num1, num2, num3};
    return {-1, -1, -1};
}

void viewGame(GameData &data, const int height, const int width){
    const int begin_y = 4, begin_x = 0;
    
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