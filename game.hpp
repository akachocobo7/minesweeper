#include "minesweeper.hpp"
#include <vector>
#include <utility>

struct inputData{
    int op, y, x;
};

void mainGame(const int height, const int width, const int mine_num);
inputData inputProcess(const int height, const int width, int &cur_y, int &cur_x); // 入力処理
void viewGame(GameData &data, const int height, const int width); // ゲーム画面描画
void getBeginDrawCell(int &begin_y, int &begin_x, int height, int width); // ゲーム画面を描き始める一番左上の座標をポインタで代入