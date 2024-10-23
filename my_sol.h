#ifndef MY_SOL_H
#define MY_SOL_H

#include <nlohmann/json.hpp>
#include <vector>
#include <deque>
#include <string>
#include "util.h"
#include "main.h"

using json = nlohmann::json;

constexpr uint64_t history_size = 1024 * 1024 * 64;
// constexpr uint64_t history_size = 256;
constexpr uint64_t history_mask = history_size - 1;

const int solider_point = 100;
const int king_point = 350;
constexpr int16_t val_unknown = INT16_MAX;
constexpr int16_t POINT_INF = INT16_MAX - 1;
constexpr int8_t flag_empty = 0;
constexpr int8_t flag_exact = 1;
constexpr int8_t flag_alpha = 2;
constexpr int8_t flag_beta = 3;
struct History {
    History() : is_white(false), round(0), flags(0), depth(0), value(0), key(0), check_sum(0) {};

    bool is_white;
    int8_t round;
    int8_t flags;
    int8_t depth;
    int16_t value;
    int_fast64_t key;
    int_fast64_t check_sum;
};

using AllHistory = std::vector<History>;
using BestKeyWindow = std::deque<int_fast64_t>;

void init_zobrist();
void init_2();
uint64_t timeSinceEpochMillisec();
class MySolution {
public:
    MySolution(int max_depth = 14, int max_step_ms = 50000, int begin_depth = 6) : max_depth(max_depth), max_step_ms(max_step_ms), begin_depth(begin_depth) {
        this->all_history.resize(history_size, History());
    };

    static void get_board(const json & response, Board & board);
    static void get_board(const json & response, Board & board, int_fast64_t &hash_key);
    static void calc_hash_key(const Board & board, int_fast64_t & hash_key);
    static void transfer_move(const Move &move, JsonMove &json_move);
    static void print_board(const json & response);
    static void print_board(const Board &board);
    static int calc_board(const Board &board, bool is_white) noexcept;

    inline void set_data(int max_step_ms, int max_depth, int begin_depth) {
        this->max_step_ms = max_step_ms;
        this->max_depth = max_depth;
        this->begin_depth = begin_depth;
        log(string_format("set sol max_step_ms:%d max_depth:%d begin_depth%d",
            this->max_step_ms, this->max_depth, this->begin_depth));
    }
    void print_status();
    //　获得最好走法
    Move get_best_move(Board &board, bool is_white);
    // 走ＸＸ走法
    void do_move(Board &board, const Move &move, MoveOps &ops, bool is_white);
    void do_move2(Board &board, const Move &move, MoveOps &ops, int_fast64_t &hash_key, int_fast64_t &check_sum, bool is_white);
    void undo_move(Board &board, MoveOps &ops);
    // undo 计算分数，需要知道自己的子
    void undo_move2(Board &board, MoveOps &ops, int_fast64_t &hash_key, int_fast64_t &check_sum);
    // alpha beta func
    void record_history(int_fast64_t hash_key, int_fast64_t check_sum, int depth, int val, int hash_type, bool is_white) noexcept;
    inline int find_history(int_fast64_t hash_key, int_fast64_t check_sum, int depth, int beta, bool is_white) noexcept;
    int alpha_beta2(Board & board, int_fast64_t hash_key, int_fast64_t check_sum, bool is_white, int alpha, int beta, int depth);
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
    // 是否需要中断
    bool is_need_break;
    // 最大搜索深度
    int max_depth;
    // 最大单步搜索时间
    uint64_t max_step_ms;
    // 开始搜索深度
    int begin_depth;
    // 检测中断层次
    int break_depth;
    // 当前搜索深度
    int cur_max_depth;
    // 最大搜索深度
    int real_max_depth;
    // 当前轮次
    int round = 0;
    // 当前轮次开始时间
    uint64_t round_begin_ms = timeSinceEpochMillisec();
    // 当前轮次最佳得分
    int best_point;
    // 最佳走法
    Move best_move;
    // 历史表，避免搜索重复局面。
    AllHistory all_history;
    // 随机种子
    // 我方先进入循环。
    // E6-E5 白 (重复局面)
    // B3-C3 黑
    // C4-D4 白
    // C3-B3
    // D4-C4 白 (重复局面)
    // B3-C3
    // C4-D4 白
    // C3-B3
    // D4-C4 白 (重复局面)
    // int_fast64_t return_board_key = 0;
    // BestKeyWindow best_key_window;
#ifdef DEBUG
    // 下面的字段 release 不修改
    // 上个reduction,　开始前赋值为cur_reduction
    int last_reduction = 0;
    // 当前reduction
    int cur_reduction = 0;
    // hash_key 命中次数
    int hash_key_return = 0;
    // beta截断次数
    int beta_cut = 0;
#endif
};

#endif