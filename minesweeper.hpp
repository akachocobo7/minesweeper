#include <vector>
#include <utility>

constexpr const int MINE = -1;

class MineSweeper{
public:
    const int height, width; // フィールドの縦サイズと横サイズ
    const int mine_num; // 地雷の数
    std::vector< std::vector<int> > field;
    std::vector< std::vector<bool> > is_opened; // そのマスが開かれたかどうか
    
    MineSweeper(const int height, const int width, const int mine_num);
    
    void initGame();
    std::vector< std::pair<int, int> > generateMinesPos();
    void createField(std::vector< std::pair<int, int> > &mines_pos);
    
    bool isGameOver(const int y, const int x, const bool is_firsthand);
    void moveMinePos(int y, int x);
    
    void openCell(const int y, const int x, const bool is_gameover);
    
    bool isGameClear();
};