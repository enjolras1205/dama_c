#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "util.h"

using json = nlohmann::json;
class MySolution {
public:
    static void get_board(const json & response, Board & board);
    // 获得棋盘上所有走法
    void get_moves(Board &board, Moves& moves, bool white);
    // dfs吃子
    // board: 棋盘
    // moved: 已走点
    // moves: 发现的移动
    // move: 当前移动，即path
    // eat_pos: 下个要吃的点
    // direction: 当前吃子的跳转方向
    // is_white: 是否白棋
    // is_king: 是否王棋
    bool get_eat_moves(const Board &board, BoardFlag &moved, Moves &moves,
        Move &move, int eat_pos, int direction, bool is_white, bool is_king);
    // 获得某个棋子可能的所有移动
    void get_press_moves(const Board &board, Moves &moves, int idx);
private:
    // 当前轮次
    int round;
    // 上个reduction,　开始前赋值为cur_reduction
    int last_reduction;
    // 当前reduction
    int cur_reduction;
};
