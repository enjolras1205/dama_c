#include "my_sol.h"
#include "unit_test.h"
#include <algorithm>
#include <random>
using namespace std;

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
        board[idx] = v["value"];
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

int MySolution::calc_board(const Board & board, bool is_white)
{
    int point = 0;
    int idx = 0;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            auto chess = board[idx];
            if (chess == empty_chess) {
                idx += 1;
                continue;
            }
            bool is_chess_white = true;
            bool is_chess_king = false;
            switch (chess)
            {
            case white_soldier: {
                break;
            }
            case white_king: {
                is_chess_king = true;
                break;
            }
            case black_soldier: {
                is_chess_white = false;
                break;
            }
            case black_king: {
                is_chess_white = false;
                is_chess_king = true;
                break;
            }
            default:
                break;
            }
            int change_point = solider_point;
            if (is_chess_king) {
                change_point = king_point;
            }
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
    // for (auto round = 1; round <= this->max_round; ++round) {
    //     this->max_depth = round * 2;
    //     MySolution::print_board(board);
    //     MySolution::print_board(board);
    // }
    this->alpha_beta(board, is_white, -INT32_MAX, INT32_MAX, this->max_depth);
    return this->best_move;
}

void MySolution::do_move(Board & board, const Move & move, MoveOps & ops, bool is_white)
{
    // 拿走起始的棋子
    ops.clear();
    auto start_idx = move[0];
    auto start_chess = board[start_idx];
    ops.push_back(MoveOp({-start_idx, board[start_idx]}));
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
                ops.push_back(MoveOp({-idx, board[idx]}));
                board[idx] = empty_chess;
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
    board[end_idx] = start_chess;
}

void MySolution::undo_move(Board & board, MoveOps & ops)
{
    for (MoveOps::const_reverse_iterator r_iter = ops.rbegin(); r_iter != ops.rend(); ++r_iter) { 
        auto idx = (*r_iter)[0];
        auto chess = (*r_iter)[1];
        if (idx < 0) {
            board[-idx] = chess;
        } else {
            board[idx] = empty_chess;
        }
    } 
}

int MySolution::alpha_beta(Board &board, bool is_white, int alpha, int beta, int depth) {
    if (depth == 0) {
        return MySolution::calc_board(board, is_white);
    }

    // 生成全部走法;
    Moves moves;
    this->get_moves(board, moves, is_white);
    if (moves.size() == 0) {
        // 没有棋走了．输
        return this->max_depth - depth - INT32_MAX;
    }

    // TODO:按历史表排序全部走法; 先随机
    std::shuffle(std::begin(moves), std::end(moves), std::default_random_engine(this->seed));
    int best_idx;
    MoveOps ops;
    for (int i = 0; i < moves.size(); ++i) {
        this->do_move(board, moves[i], ops, is_white);
        int current_point = -alpha_beta(board, !is_white, -beta, -alpha, depth - 1);
        this->undo_move(board, ops);
        if (current_point >= beta) {
            // TODO:记录到历史表
            return beta;
        }
        if (current_point > alpha) {
            alpha = current_point;
            best_idx = i;
        }
    }

    // TODO:记录最佳走法到历史表
    if (depth == this->max_depth) {
        this->best_move = moves[best_idx];
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
