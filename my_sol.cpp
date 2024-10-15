#include "my_sol.h"
#include "unit_test.h"
#include <algorithm>
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
    auto &board_data = response["board"];
    for (auto &v : board_data) {
        const char a = 'a';
        const string &idx_1 = v["index"][0];
        const string &idx_2 = v["index"][1];
        uint16_t idx = 0;
        idx += (idx_1[0] - a);
        idx += (BOARD_BOUND_SIZE - std::stoi(idx_2)) * 16;
        board[idx] = v["value"];
    }
}

void MySolution::get_moves(Board &board, Moves &moves, bool is_white)
{
    moves.clear();
    // 找到所有棋子
    // 排序这些moves
    int idx = 0;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            // 水平移动
            // 0,1,2,3,4,5,6,7 ...
            auto chess = board[idx];
            bool is_my = is_my_chess(is_white, chess);
            if (is_my) {
                this->get_press_moves(board, moves, idx);
            }
            idx += 1;
        }
        cout << std::endl;
        // 垂直移动
        idx += BOARD_BOUND_SIZE;
    }
}

bool MySolution::get_eat_moves(const Board &board, BoardFlag &moved, Moves &moves,
    Move &move, int eat_pos, int direction, bool is_white, bool is_king)
{
    if (moved[eat_pos]) {
        return false;
    }
    int next_pos = eat_pos + direction;
    // 兵吃子是跳到棋子后一格．王可能跳到任意格．
    // 如果跳的方向不变．
    bool eaten = false;
    bool is_direction_block = false;
    while (!(next_pos & 0x88)) {
        int pos_type = get_board_pos_type(board, next_pos);
        switch (pos_type) {
            case pos_empty: {
                // 空位，可以吃，继续寻找下一个跳跃点。
                MoveDirections directions;
                if (is_king) {
                    directions = MoveDirections{-1, 1, BOARD_LINE_SIZE, -BOARD_LINE_SIZE};
                } else if (is_white) {
                    directions = MoveDirections{-1, 1, BOARD_LINE_SIZE};
                } else {
                    directions = MoveDirections{-1, 1, -BOARD_LINE_SIZE};
                }

                Move new_move = move;
                new_move.push_back(next_pos);
                moved[eat_pos] = true;
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
                            bool this_eaten = this->get_eat_moves(board, moved, moves, 
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
                moved[eat_pos] = false;
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

void MySolution::get_press_moves(const Board & board, Moves & moves, int idx)
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
        directions = MoveDirections{-1, 1, BOARD_LINE_SIZE, -BOARD_LINE_SIZE};
    } else if (is_white) {
        directions = MoveDirections{-1, 1, BOARD_LINE_SIZE};
    } else {
        directions = MoveDirections{-1, 1, -BOARD_LINE_SIZE};
    }

    Moves eat_moves;
    bool is_direction_block = false;
    for (int direction : directions) {
        int next_pos = idx + direction;
        while (!(next_pos & 0x88)) {
            int next_pos_type = get_board_pos_type(board, next_pos);
            switch (next_pos_type) {
                case pos_empty: {
                    moves.push_back(Move{idx, next_pos});
                    break;
                }
                case pos_white: {
                    is_direction_block = true;
                    if (!is_white) {
                        BoardFlag flags; 
                        Move move = {idx};
                        this->get_eat_moves(board, flags, eat_moves, move, next_pos, direction, is_white, is_king);
                    }
                    break;
                }
                case pos_black: {
                    is_direction_block = true;
                    if (is_white) {
                        BoardFlag flags; 
                        Move move = {idx};
                        this->get_eat_moves(board, flags, eat_moves, move, next_pos, direction, is_white, is_king);
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

    Moves max_eat_moves;
    get_max_eat_moves(eat_moves, max_eat_moves);
    moves.insert(moves.end(), max_eat_moves.begin(), max_eat_moves.end());
    return;
}
