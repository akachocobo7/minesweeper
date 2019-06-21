#include <ncurses.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    initscr(); // ncursesの初期化
    printw("Hello World!\n");
    sleep(3);
    endwin(); // ncurses就労
}