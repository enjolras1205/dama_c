#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "util.h"

using json = nlohmann::json;
class MySolution {
public:
    static void get_board(const json & response, Board & board);
    static void transfer_move(const Move &move, JsonMove &json_move);
    static void print_board(const json & response);
    static void print_board(const Board &board);
    static int calc_board(const Board &board, bool is_white);

    //　获得最好走法
    Move get_best_move(Board &board, bool is_white);
    // 走ＸＸ走法
    void do_move(Board &board, const Move &move, MoveOps &ops, bool is_white);
    void undo_move(Board &board, MoveOps &ops);
    // alpha beta func
    int alpha_beta(Board & board, bool is_white, int alpha, int beta, int depth);
    // 获得棋盘上所有走法
    void get_moves(Board &board, Moves& moves, bool is_white);
    // dfs吃子
    // board: 棋盘
    // moved: 已走点
    // moves: 发现的移动
    // move: 当前移动，即path
    // eat_pos: 下个要吃的点
    // direction: 当前吃子的跳转方向
    // is_white: 是否白棋
    // is_king: 是否王棋
    bool get_eat_moves(Board &board, BoardFlag &moved, Moves &moves,
        Move &move, int eat_pos, int direction, bool is_white, bool is_king);
    // 获得某个棋子可能的所有移动，返回是否有吃子
    bool get_press_moves(Board &board, Moves &moves, int idx);
private:
    // 最佳走法
    Move best_move;
    // 最大搜索深度
    int max_depth = 8;
    int max_round = 2;
    // 当前轮次
    int round;
    // 上个reduction,　开始前赋值为cur_reduction
    int last_reduction;
    // 当前reduction
    int cur_reduction;
};
