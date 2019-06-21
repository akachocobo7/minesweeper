#include <vector>
#include <utility>

constexpr const int MINE = -1;

// ゲーム画面の描画に必要なデータ
struct GameData{
    std::vector< std::vector<int> > field;
    std::vector< std::vector<bool> > is_opened;
    bool is_gameover, is_gameclear;
    
    GameData(){};
    GameData(std::vector< std::vector<int> > &field,
             std::vector< std::vector<bool> > &is_opened,
             bool is_gameover, bool is_gameclear) : field(field),
                                                    is_opened(is_opened),
                                                    is_gameover(is_gameover),
                                                    is_gameclear(is_gameclear){};
};

class MineSweeper{
private:
    const int height, width; // フィールドの縦サイズと横サイズ
    const int mine_num; // 地雷の数
    bool is_firsthand; // 初手かどうかのフラグ
    bool is_gameover, is_gameclear;
    std::vector< std::vector<int> > field;
    std::vector< std::vector<bool> > is_opened; // そのマスが開かれたかどうか
    
    std::vector< std::pair<int, int> > generateMinesPos();
    void createField(std::vector< std::pair<int, int> > &mines_pos);
    
    bool isGameOver(const int y, const int x);
    void moveMinePos(int y, int x);
    void openCell(const int y, const int x, const bool is_gameover);
    bool isGameClear();
    
public:
    MineSweeper(const int height, const int width, const int mine_num);
    void initGame(); // ゲームの初期化
    void openCell(const int y, const int x); // (y, x)のマスを開く
    GameData getGameData(); // フィールドの情報とゲームオーバーorクリアしたかの情報を得る
};