#include "my_sol.h"
#include "main.h"
#include "my_sol_v1.h"
#include "unit_test.h"
#include <algorithm>
#include <random>
using namespace std;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

bool my_move_sort_func(const MovePoint &a, const MovePoint &b) { return (a.first > b.first); }

int_fast64_t g_zobrist[black_king + 1][BOARD_SIZE];

// backup
// int16_t g_pos_point[black_king + 1][BOARD_SIZE] = {
//     // 空
//     {
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//     },
//     // 白兵
//     {
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
//         2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         4,4,4,4,4,4,4,4,-1,-1,-1,-1,-1,-1,-1,-1,
//         6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
//         6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
//     },
//     // 白王
//     {
//         9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         9,4,4,4,4,4,4,9,-1,-1,-1,-1,-1,-1,-1,-1,
//     },
//     // 黑兵
//     {
//         6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
//         6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
//         4,4,4,4,4,4,4,4,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,
//         1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//         0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
//     },
//     // 黑王
//     {
//         9,4,4,4,4,4,4,9,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
//         9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
//     },
// };
// 兵位置价值
int16_t g_pos_point[black_king + 1][BOARD_SIZE] = {
    // 空
    {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    },
    // 白兵
    {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        3,2,2,2,2,2,2,3,-1,-1,-1,-1,-1,-1,-1,-1,
        4,3,3,3,3,3,3,4,-1,-1,-1,-1,-1,-1,-1,-1,
        5,4,4,4,4,4,4,5,-1,-1,-1,-1,-1,-1,-1,-1,
        6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
        6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
    },
    // 白王
    {
        9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
    },
    // 黑兵
    {
        6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
        6,6,6,6,6,6,6,6,-1,-1,-1,-1,-1,-1,-1,-1,
        5,4,4,4,4,4,4,5,-1,-1,-1,-1,-1,-1,-1,-1,
        4,3,3,3,3,3,3,4,-1,-1,-1,-1,-1,-1,-1,-1,
        3,2,2,2,2,2,2,3,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    },
    // 黑王
    {
        9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        9,3,3,3,3,3,3,9,-1,-1,-1,-1,-1,-1,-1,-1,
    },
};

void init_pos_point() {
  for (int j = 0; j < BOARD_SIZE; ++j) {
    g_pos_point[white_soldier][j] += solider_point;
  }
  for (int j = 0; j < BOARD_SIZE; ++j) {
    g_pos_point[white_king][j] += king_point;
  }
  for (int j = 0; j < BOARD_SIZE; ++j) {
    g_pos_point[black_soldier][j] += solider_point;
  }
  for (int j = 0; j < BOARD_SIZE; ++j) {
    g_pos_point[black_king][j] += king_point;
  }
}

void init_2() {
    init_zobrist();
    init_pos_point();
}

void init_zobrist() {
  std::mt19937_64 gen(8036732692983876157);
  std::uniform_int_distribution<uint64_t> dis;
  for (int i = 0; i <= black_king; ++i) {
    for (int j = 0; j < BOARD_SIZE; ++j) {
      auto v = dis(gen);
      g_zobrist[i][j] = v;
    }
  }
}


inline void get_max_eat_moves(const Moves &moves, Moves &max_moves) {
    size_t max_size = 0;
    for (auto &v : moves) {
        if (v.size() > max_size) {
            max_moves = {v};
            max_size = v.size();
        } else if (v.size() == max_size) {
            max_moves.push_back(v);
        }
    }
}

// 将服务器返回转成 0x88 棋盘
// [{"value": 0, "index": ["a", "8"]}, ...]
void MySolution::get_board(const json &response, Board &board)
{
    auto &board_data = response;
    const char a = 'a';
    for (auto &v : board_data) {
        const string &idx_1 = v["index"][0];
        const string &idx_2 = v["index"][1];
        uint16_t idx = 0;
        idx += (idx_1[0] - a);
        idx += (BOARD_BOUND_SIZE - std::stoi(idx_2)) * 16;
        ChessType value = v["value"];
        board[idx] = value;
    }
}

void MySolution::calc_hash_key(const Board &board, int_fast64_t &hash_key)
{
    hash_key = g_zobrist[0][0];
    int idx = 0;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            auto chess = board[idx];
            if (chess <= empty_chess) {
                idx += 1;
                continue;
            }
            hash_key ^= g_zobrist[chess][idx];
            idx += 1;
        }
        idx += BOARD_BOUND_SIZE;
    }
}

void MySolution::transfer_move(const Move & move, JsonMove &json_move)
{
    json_move.clear();
    const char a = 'a';
    for (auto v : move) {
        char line = a + (v % BOARD_LINE_SIZE);
        int row = BOARD_BOUND_SIZE - v / BOARD_LINE_SIZE;

        json_move.push_back({std::string(1, line), std::to_string(row)});
    }
}

void MySolution::print_board(const json & response)
{
    Board board;
    MySolution::get_board(response, board);
    MySolution::print_board(board);
}

void MySolution::print_board(const Board & board)
{
    // return;
    int idx = 0;
    while (!(idx & 0x88)) {
        int row = BOARD_BOUND_SIZE - idx / BOARD_LINE_SIZE;
        int line_start = (BOARD_BOUND_SIZE - row) * BOARD_LINE_SIZE;
        cout << std::setw(4) << line_start << "_" << row << ": ";
        while (!(idx & 0x88)) {
            // 水平移动
            // 0,1,2,3,4,5,6,7 ...
            auto chess = board[idx];
            cout << std::to_string(chess) << " ";
            idx += 1;
        }
        cout << std::endl;
        // 垂直移动
        idx += BOARD_BOUND_SIZE;
    }
    cout << "        a b c d e f g h" << std::endl;
}

int MySolution::calc_board(const Board & board, bool is_white) noexcept
{
    // return MySolutionV1::calc_board(board, is_white);
    int point = 0;
    int idx = 0;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            auto chess = board[idx];
            if (chess == empty_chess) {
                idx += 1;
                continue;
            }
            bool is_chess_white = is_white_chess(chess);
            int change_point = g_pos_point[chess][idx];

            if (is_white == is_chess_white) {
                point += change_point;
            } else {
                point -= change_point;
            }
            idx += 1;
        }
        idx += BOARD_BOUND_SIZE;
    }

    return point;
}


Move MySolution::get_best_move(Board & board, bool is_white)
{
    int_fast64_t hash_key = 0;
    int_fast64_t check_sum = g_zobrist[0][1];;
    MySolution::calc_hash_key(board, hash_key);
#ifdef DEBUG
    this->last_reduction = this->cur_reduction;
#endif
    this->round += 1;
    this->round_begin_ms = timeSinceEpochMillisec();
    int depth = this->begin_depth;
    this->break_depth = -1;
    this->is_need_break = false;
    do {
        this->cur_max_depth = depth;
        this->alpha_beta2(board, hash_key, check_sum, is_white, -POINT_INF, POINT_INF, depth);
        if (this->is_need_break) {
            break;
        }
        if (depth >= 10) {
            depth += 1;
        } else {
            depth += 2;
        }
        // 至少做一轮 alpha beta 有一个基本的结果。
        this->break_depth = this->begin_depth;
    } while (depth <= this->max_depth);

    auto end_time = timeSinceEpochMillisec();
    int board_point = MySolution::calc_board(board, is_white);
    json log_data = {
        {"version", "hehe_v1_7"},
        {"move", this->best_move},
        {"round", this->round},
        {"try_depth", this->cur_max_depth},
        {"real_depth", this->real_max_depth},
        {"best_point", this->best_point},
        {"is_white", is_white},
        {"is_break", this->is_need_break},
        {"board_point", board_point},
        {"max_step_ms", this->max_step_ms},
        {"time_in_ms", end_time - this->round_begin_ms}
    };

    log((std::string)"stat:\n"+ log_data.dump());
    // 记录走过的局面
    // this->best_key_window.push_back(this->return_board_key);

    // if (this->best_key_window.size() >= 10) {
    //     this->best_key_window.pop_front();
    // }

    return this->best_move;
}

void MySolution::print_status() {
    std::cout<<"round:"<< this->round <<std::endl;
    std::cout<<"cur_max_depth:"<< this->cur_max_depth <<std::endl;
    std::cout<<"best_point:"<< this->best_point<<std::endl;
#ifdef DEBUG
    std::cout<<"reduction:"<< this->cur_reduction - this->last_reduction <<std::endl;
    std::cout<<"total_reduction:"<< this->cur_reduction <<std::endl;
    std::cout<<"total_hash_hit:"<< this->hash_key_return <<std::endl;
    std::cout<<"beta_cut:"<< this->beta_cut <<std::endl;
#endif
}

void MySolution::do_move(Board & board, const Move & move, MoveOps & ops, bool is_white)
{
    int_fast64_t hash_key = 0;
    int_fast64_t check_sum = 0;
    this->do_move2(board, move, ops, hash_key, check_sum, is_white);
}

void MySolution::do_move2(Board & board, const Move & move, MoveOps & ops, 
    int_fast64_t & hash_key, int_fast64_t & check_sum, bool is_white)
{
    // 拿走起始的棋子
    ops.clear();
    auto start_idx = move[0];
    auto start_chess = board[start_idx];
    hash_key ^= g_zobrist[start_chess][start_idx];
    check_sum ^= g_zobrist[start_chess][start_idx];
    ops.push_back(MoveOp({start_idx, -start_chess}));
    board[start_idx] = empty_chess;
    // 拿走吃的棋子
    for (size_t i = 1; i < move.size(); ++i) {
        auto start_idx = move[i - 1];
        auto end_idx = move[i];
        if (end_idx < start_idx) {
            std::swap(start_idx, end_idx);
        }
        // 默认水平遍历
        int direction = 1;
        if (end_idx - start_idx > BOARD_BOUND_SIZE) {
            // 垂直
            direction = BOARD_LINE_SIZE;
        }
        auto idx = start_idx + direction;
        while (idx < end_idx) {
            // 如果有对方棋子
            int chess = board[idx];
            if (chess != empty_chess && !is_my_chess(is_white, chess)) {
                ops.push_back(MoveOp({idx, -board[idx]}));
                board[idx] = empty_chess;
                hash_key ^= g_zobrist[chess][idx];
                check_sum ^= g_zobrist[chess][idx];
            }
            idx += direction;
        }
    }

    // 把开始的棋子放在结束处
    auto end_idx = move.back();
    // 判断是否需要变王
    if (start_chess == white_soldier && end_idx >= BOARD_LAST_LINE_START) {
        start_chess = white_king;
    }
    if (start_chess == black_soldier && end_idx < BOARD_BOUND_SIZE) {
        start_chess = black_king;
    }
    ops.push_back(MoveOp({end_idx, start_chess}));
    hash_key ^= g_zobrist[start_chess][end_idx];
    check_sum ^= g_zobrist[start_chess][end_idx];
    board[end_idx] = start_chess;
}

void MySolution::undo_move(Board & board, MoveOps & ops)
{
    int_fast64_t hash_key = 0;
    int_fast64_t check_sum = 0;
    this->undo_move2(board, ops, hash_key, check_sum);
}

void MySolution::undo_move2(Board & board, MoveOps & ops, int_fast64_t &hash_key, int_fast64_t &check_sum)
{
    for (MoveOps::const_reverse_iterator r_iter = ops.rbegin(); r_iter != ops.rend(); ++r_iter) { 
        auto idx = (*r_iter)[0];
        auto chess = (*r_iter)[1];
        bool is_push_back = false;
        if (chess < 0) {
           is_push_back = true; 
           chess = -chess;
        }

        hash_key ^= g_zobrist[chess][idx];
        check_sum ^= g_zobrist[chess][idx];
        if (is_push_back) {
            board[idx] = chess;
        } else {
            board[idx] = empty_chess;
        }
    } 
}

void MySolution::record_history(int_fast64_t hash_key, int_fast64_t check_sum, int depth, int val, int hash_flags, bool is_white) noexcept
{
    auto idx = hash_key & history_mask;
    History &his = this->all_history[idx];
    if (his.depth > depth && this->round == his.round) {
        // 本轮已经记录更深的历史
        return;
    }
    his.round = this->round;
    his.key = hash_key;
    his.check_sum = check_sum;
    his.value = val;
    his.flags = hash_flags;
    his.depth = depth;
    his.is_white = is_white;
}

inline int MySolution::find_history(int_fast64_t hash_key, int_fast64_t check_sum, int depth, int beta, bool is_white) noexcept
{
    auto idx = hash_key & history_mask;
    auto ret = val_unknown;
    History &his = this->all_history[idx];
    auto his_val = his.value;

    if (his.key == hash_key && his.check_sum == check_sum && his.round == this->round) {
        if (his.is_white != is_white) {
            his_val = -his_val;
        }
#ifdef DEBUG
        this->hash_key_return += 1;
#endif
        if (his.depth >= depth) {
            if (his.flags == flag_exact) {
                return his_val;
            }
            // if ((his.flags == flag_alpha) && (his_val <= alpha)) {
            //     return alpha;
            // }
            if ((his.flags == flag_beta) && (his_val >= beta)) {
                return beta;
            }
        }
#ifdef DEBUG
        this->hash_key_return -= 1;
#endif
    }

    return ret;
}

int MySolution::alpha_beta2(Board &board, int_fast64_t hash_key, int_fast64_t check_sum, bool is_white, int alpha, int beta, int depth) 
{
    auto val = this->find_history(hash_key, check_sum, depth, beta, is_white);
    if (val != val_unknown) {
        return val;
    }

    if (depth == 0) {
        int board_point = MySolution::calc_board(board, is_white);
        this->record_history(hash_key, check_sum, depth, board_point, flag_exact, is_white);
        return board_point;
    } else if (depth == this->break_depth) {
        auto end_time = timeSinceEpochMillisec();
        if ((end_time - this->round_begin_ms) > this->max_step_ms) {
            this->is_need_break = true;
            return val;
        }
    }

    // 生成全部走法;
    Moves moves;
    this->get_moves(board, moves, is_white);
    if (moves.size() == 0) {
        // 没有棋走了．输
        return this->max_depth - depth - POINT_INF;
    } else if(moves.size() == 1 && depth == this->cur_max_depth) {
        this->best_move = moves[0];
        // 这里不记录历史表。第一层就返回了。
        return 0;
    }

#ifdef DEBUG
    this->cur_reduction += 1;
#endif

    // 发现排序没什么卵用，可能因为强制吃子的设定，大部分移动的价值都是0.
    // MovePoints move_points;
    // for (const auto & v: moves) {
    //     this->do_move2(board, v, ops, hash_key, point, is_white);
    //     // 获得执行move后的局面分
    //     // 急需开局库让搜索更有效，大部分时候都不命中
    //     auto move_val = this->find_history(hash_key, -1, -beta, -alpha, !is_white);
    //     if (move_val == val_unknown) {
    //         move_val = 0;
    //     }
    //     this->undo_move2(board, ops, hash_key, point, is_white);
    //     move_points.push_back(MovePoint{move_val, v});
    // }
    // std::sort(move_points.begin(), move_points.end(), my_move_sort_func);

    // 本身就DFS优先选择向前移动。向前吃子冲突概率更高，更容易产生截断。
    // 最顶层还是随机一下，因为评估一样的局面不一定真的一样。更随机的结果可以减少和棋的概率。
    if (depth == this->cur_max_depth) {
        std::shuffle(std::begin(moves), std::end(moves), this->random_engine);
    }
    size_t best_idx = 0;
    MoveOps ops;
    int_fast64_t best_hash_key = 0;
    for (size_t i = 0; i < moves.size(); ++i) {
        this->do_move2(board, moves[i], ops, hash_key, check_sum, is_white);
        int_fast64_t tmp = hash_key;
        // 如果在最大层, 发现重复局面
        // 1.局面出现2次
        // 2.有其它选择. 第一层一定有alpha，而且这里有多个moves，所以不需要判断了。
        // 记得回滚！！！
        // if (depth == this->cur_max_depth) {
        //     int duplicate_count = 0;
        //     for (auto v: this->best_key_window) {
        //         if (v == tmp) {
        //             duplicate_count += 1;
        //         }
        //     }
        //     if (duplicate_count >= 2) {
        //         this->undo_move2(board, ops, hash_key);
        //         continue;
        //     }
        // }

        int current_point = -this->alpha_beta2(board, hash_key, check_sum, !is_white, -beta, -alpha, depth - 1);
        this->undo_move2(board, ops, hash_key, check_sum);
        if (this->is_need_break) {
            return current_point;
        }
        if (current_point >= beta) {
            this->record_history(hash_key, check_sum, depth, beta, flag_beta, is_white);
#ifdef DEBUG
            this->beta_cut += 1;
#endif
            return beta;
        }
        if (current_point > alpha) {
            alpha = current_point;
            best_hash_key = tmp;
            best_idx = i;
        }
    }

    this->record_history(best_hash_key, check_sum, depth, alpha, flag_exact, is_white);
    if (depth == this->cur_max_depth) {
        this->best_move = moves[best_idx];
        this->best_point = alpha;
        this->real_max_depth = depth;
        // this->return_board_key = best_hash_key;
    }

    return alpha;
}

void MySolution::get_moves(Board &board, Moves &moves, bool is_white)
{
    moves.clear();
    // 找到所有棋子
    // 排序这些moves
    int idx = 0;
    bool has_eaten = false;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            // 水平移动
            // 0,1,2,3,4,5,6,7 ...
            auto chess = board[idx];
            bool is_my = is_my_chess(is_white, chess);
            if (is_my) {
                Moves one_press_moves;
                bool is_eaten = this->get_press_moves(board, one_press_moves, idx);
                if (!has_eaten && is_eaten) {
                    // 之前没吃子，新吃子
                    moves = std::move(one_press_moves);
                    has_eaten = true;
                } else if(is_eaten || !has_eaten) {
                    moves.insert(moves.end(), one_press_moves.begin(), one_press_moves.end()); 
                }
            }
            idx += 1;
        }
        // 垂直移动
        idx += BOARD_BOUND_SIZE;
    }
    if (has_eaten) {
        Moves max_eat_moves;
        get_max_eat_moves(moves, max_eat_moves);
        moves = std::move(max_eat_moves);
    }
}

bool MySolution::get_eat_moves(Board &board, Moves &moves,
    Move &move, int eat_pos, int direction, bool is_white, bool is_king)
{
    int next_pos = eat_pos + direction;
    // 兵吃子是跳到棋子后一格．王可能跳到任意格．
    // 如果跳的方向不变．
    bool eaten = false;
    while (!(next_pos & 0x88)) {
        bool is_direction_block = false;
        int pos_type = get_board_pos_type(board, next_pos);
        switch (pos_type) {
            case pos_empty: {
                // 空位，可以吃，继续寻找下一个跳跃点。
                MoveDirections directions;
                if (is_king) {
                    directions = MoveDirections{BOARD_LINE_SIZE, -BOARD_LINE_SIZE, -1, 1};
                } else if (is_white) {
                    directions = MoveDirections{BOARD_LINE_SIZE, -1, 1};
                } else {
                    directions = MoveDirections{-BOARD_LINE_SIZE, -1, 1};
                }

                Move new_move = move;
                new_move.push_back(next_pos);
                auto chess_backup = board[eat_pos];
                board[eat_pos] = empty_chess;
                eaten = true;
                bool still_eaten = false;
                for (int new_direction : directions)
                {
                    // 不能１８０度掉头
                    if (new_direction == -direction) {
                      continue;
                    }
                    int next_eat_pos = next_pos + new_direction;
                    while (!(next_eat_pos & 0x88)) {
                        int next_eat_pos_type = get_board_pos_type(board, next_eat_pos);
                        if ((next_eat_pos_type == pos_black && is_white) || (next_eat_pos_type == pos_white && !is_white)) {
                            bool this_eaten = this->get_eat_moves(board, moves, 
                              new_move, next_eat_pos, new_direction, is_white, is_king);
                            still_eaten = still_eaten || this_eaten;
                        }
                        if (!is_king || next_eat_pos_type == pos_black || next_eat_pos_type == pos_white) {
                          break;
                        }
                        next_eat_pos += new_direction;
                    }
                }
                if (!still_eaten) {
                    // 吃不动了。记录最长的跳跃路径。
                    moves.push_back(new_move);
                }
                board[eat_pos] = chess_backup;
                break;
            }
            default: {
                // 自己子，对方子，棋盘外，不是可吃的子， 停止迭代
                is_direction_block = true;
                break;
            }
        }

        if (!is_king || is_direction_block) {
            break;
        }
        next_pos += direction;
    }

    return eaten;
}

bool MySolution::get_press_moves(Board & board, Moves & moves, int idx)
{
    bool is_white = true;
    bool is_king = false;
    int chess = board[idx];

    switch (chess) {
        case white_soldier: {
            break;
        }
        case white_king: {
            is_king = true;
            break;
        }
        case black_soldier: {
            is_white = false;
            break;
        }
        case black_king: {
            is_white = false;
            is_king = true;
            break;
        }
        default: {
            std::cout << "error1: unknown press type" << endl;
            break;
        }
    }

    MoveDirections directions;
    if (is_king) {
        directions = MoveDirections{BOARD_LINE_SIZE, -BOARD_LINE_SIZE, -1, 1};
    } else if (is_white) {
        directions = MoveDirections{BOARD_LINE_SIZE, -1, 1};
    } else {
        directions = MoveDirections{-BOARD_LINE_SIZE, -1, 1};
    }

    Moves eat_moves;
    bool is_eaten = false;
    for (int direction : directions) {
        bool is_direction_block = false;
        int next_pos = idx + direction;
        while (!(next_pos & 0x88)) {
            int next_pos_type = get_board_pos_type(board, next_pos);
            switch (next_pos_type) {
                case pos_empty: {
                    if (!is_eaten) {
                        // 如果没吃过子，记录普通移动，否则不记录．
                        moves.push_back(Move{idx, next_pos});
                    }
                    break;
                }
                case pos_white: {
                    is_direction_block = true;
                    if (!is_white) {
                        Move move = {idx};
                        is_eaten = this->get_eat_moves(board, eat_moves, move, next_pos, direction, is_white, is_king) || is_eaten;
                    }
                    break;
                }
                case pos_black: {
                    is_direction_block = true;
                    if (is_white) {
                        Move move = {idx};
                        is_eaten = this->get_eat_moves(board, eat_moves, move, next_pos, direction, is_white, is_king) || is_eaten;
                    }
                    break;
                }
                default:
                    break;
            }
            next_pos = next_pos + direction;
            if (!is_king || is_direction_block) {
                // 兵只走一步，王走到被阻挡为止．
                break;
            }
        }
    }

    if (eat_moves.size() > 0) {
        moves = std::move(eat_moves);
    }
    return is_eaten;
}
