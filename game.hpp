#include "minesweeper.hpp"
#include <vector>
#include <utility>
#include <tuple>

struct inputData{
    int op, y, x;
};

void mainGame(const int height, const int width, const int mine_num);
inputData inputProcess(const int height, const int width); // 入力処理
inputData split(char s[]);
void viewGame(GameData &data, const int height, const int width); // ゲーム画面描画