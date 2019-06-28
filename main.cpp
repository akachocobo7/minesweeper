#include <ncurses.h>
#include <unistd.h>
#include "game.hpp"

int main(int argc, char *argv[]){
    initscr(); // ncursesの初期化
    mainGame(10, 10, 10);
    endwin(); // ncurses終了
}