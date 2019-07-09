#include "minesweeper.hpp"
#include <assert.h>
#include <random>

MineSweeper::MineSweeper(const int height, const int width, const int mine_num) : height(height),
                                                                                  width(width),
                                                                                  mine_num(mine_num){}

void MineSweeper::initGame(){
    assert(mine_num < height * width); // 地雷の数はフィールドに収まる数
    
    auto mines_pos = generateMinesPos();
    createField(mines_pos);
    is_firsthand = true;
    is_gameover = is_gameclear = false;
}

std::vector< std::pair<int, int> > MineSweeper::generateMinesPos(){
    std::vector< std::pair<int, int> > vec; // 地雷を置く座標の候補
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            vec.push_back({y, x});
        }
    }
    
    std::vector< std::pair<int, int> > res; // 地雷の位置
    std::random_device seed_rnd; // シード生成機
    std::mt19937 mt(seed_rnd()); // メルセンヌツイスター 引数は初期シード
    for(int i = 0; i < mine_num; i++){
        std::uniform_int_distribution<> rnd(0, (int)vec.size() - 1); // [0, vec.size())の一様乱数
        const int idx = rnd(mt) % vec.size();
        const int y = vec[idx].first, x = vec[i].second;
        res.push_back({y, x});
        
        // 置いた座標を候補から消す
        vec[idx] = vec.back();
        vec.pop_back();
    }
    
    return res;
}

void MineSweeper::createField(std::vector< std::pair<int, int> > &mines_pos){
    field.assign(height, std::vector<int>(width, 0));
    is_opened.assign(height, std::vector<bool>(width, false));
    is_flag_placed.assign(height, std::vector<bool>(width, false));
    
    for(const auto &pos : mines_pos){
        int y = pos.first, x = pos.second;
        field[y][x] = MINE; // 地雷を設置
    }
    
    // 8近傍を探索する
    const int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0}; 
    const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    
    // 8近傍の地雷の数をフィールドに書く
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(field[y][x] != MINE){
                int mine_cnt = 0;
                for(int i = 0; i < 8; i++){
                    const int yy = y + dy[i], xx = x + dx[i]; // 移動後の座標
                    if(yy < 0 || yy >= height || xx < 0 || xx >= width || field[yy][xx] != MINE) continue;
                    mine_cnt++;
                }
                field[y][x] = mine_cnt;
            }
        }
    }
}

int MineSweeper::openCell(const int y, const int x){
    if(is_flag_placed[y][x]) return -1;
    is_gameover = isGameOver(y, x);
    is_firsthand = false;
    openCell(y, x, is_gameover);
    is_gameclear = isGameClear();
    return 0;
}

void MineSweeper::putFlag(const int y, const int x){
    if(is_opened[y][x]) return;
    is_flag_placed[y][x] = !is_flag_placed[y][x];
}

bool MineSweeper::isGameOver(const int y, const int x){
    if(field[y][x] != MINE) return false;
    
    if(is_firsthand){
        moveMinePos(y, x);
        return false;
    }
    
    return true;
}

void MineSweeper::moveMinePos(int y, int x){
    std::vector< std::pair<int, int> > vec; // 地雷を置く座標の候補
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(field[i][j] == MINE) continue;
            vec.push_back({i, j});
        }
    }
    
    std::random_device rnd; // シード生成機
    std::mt19937 mt(rnd()); // メルセンヌツイスター 引数は初期シード
    const auto new_pos = vec[mt() % vec.size()];
    const int new_pos_y = new_pos.first, new_pos_x = new_pos.second;
    field[new_pos_y][new_pos_x] = MINE; // 地雷を設置
    
    // 8近傍を探索する
    const int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0}; 
    const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    // フィールドの数字マスを再計算
    int mine_cnt = 0;
    for(int i = 0; i < 8; i++){
        const int yy = y + dy[i], xx = x + dx[i];
        if(yy < 0 || yy >= height || xx < 0 || xx >= width) continue;
        if(field[yy][xx] == MINE){
            mine_cnt++;
        }
        else{
            field[yy][xx]--;
        }
    }
    field[y][x] = mine_cnt;
    
    for(int i = 0; i < 8; i++){
        const int yy = new_pos_y + dy[i], xx =  new_pos_x + dx[i];
        if(yy < 0 || yy >= height || xx < 0 || xx >= width) continue;
        if(field[yy][xx] != MINE){
            field[yy][xx]++;
        }
    }
}

void MineSweeper::openCell(const int y, const int x, const bool is_gameover){
    if(is_gameover){
        // 全てのマスを開く
        is_opened.assign(height, std::vector<bool>(width, true));
        is_flag_placed.assign(height, std::vector<bool>(width, false));
        return;
    }
    
    // yとxがフィールド外
    if(y < 0 || y >= height || x < 0 || x >= width) return;
    
    // 既に開けられていた場合
    if(is_opened[y][x]) return;
    
    is_opened[y][x] = true;
    is_flag_placed[y][x] = false;
    
    // 空白マスなら再帰で8近傍のマスも開ける
    if(field[y][x] == 0){
        // 8近傍を探索する
        const int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0}; 
        const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
        for(int i = 0; i < 8; i++){
            openCell(y + dy[i], x + dx[i], false);
        }
    }
}

bool MineSweeper::isGameClear(){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            // 数字マスを開けていない
            if(field[y][x] != MINE && !is_opened[y][x]) return false;
            // 地雷マスを開けている
            if(field[y][x] == MINE && is_opened[y][x]) return false;
        }
    }
    // 全てのマスを開く
    is_opened.assign(height, std::vector<bool>(width, true));
    is_flag_placed.assign(height, std::vector<bool>(width, false));
    return true;
}

GameData MineSweeper::getGameData(){
    return GameData(field, is_opened, is_flag_placed, is_gameover, is_gameclear);
}