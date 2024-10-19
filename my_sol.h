#ifndef MY_SOL_H
#define MY_SOL_H

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "util.h"

using json = nlohmann::json;

void init_zobrist();
void init_2();
class MySolution {
public:
    MySolution(int max_depth = 8) : max_depth(max_depth) {};

    static void get_board(const json & response, Board & board);
    static void get_board(const json & response, Board & board, int_fast64_t &hash_key);
    static void transfer_move(const Move &move, JsonMove &json_move);
    static void print_board(const json & response);
    static void print_board(const Board &board);
    static int calc_board(const Board &board, bool is_white);

    //　获得最好走法
    Move get_best_move(Board &board, bool is_white, int_fast64_t hash_key);
    // 走ＸＸ走法
    void do_move(Board &board, const Move &move, MoveOps &ops, bool is_white);
    void do_move2(Board &board, const Move &move, MoveOps &ops, int &point, bool is_white);
    void undo_move(Board &board, MoveOps &ops);
    // undo 计算分数，需要知道自己的子
    void undo_move2(Board &board, MoveOps &ops, int &point, bool is_white);
    // alpha beta func
    int alpha_beta(Board & board, bool is_white, int alpha, int beta, int depth) noexcept;
    int alpha_beta2(Board & board, int_fast64_t hash_key, int point, bool is_white, int alpha, int beta, int depth) noexcept;
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
    bool get_eat_moves(Board &board, Moves &moves,
        Move &move, int eat_pos, int direction, bool is_white, bool is_king);
    // 获得某个棋子可能的所有移动，返回是否有吃子
    bool get_press_moves(Board &board, Moves &moves, int idx);
private:
    int max_depth;
    // 最佳走法
    Move best_move;
    // 最大搜索深度
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 当前轮次
    int round;
    // 上个reduction,　开始前赋值为cur_reduction
    int last_reduction;
    // 当前reduction
    int cur_reduction;
};

#endif