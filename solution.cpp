#include <random>
#include <time.h>
#include <cstdlib>
#include "solution.h"
#include "my_sol.h"

// 可以稳定出现移动错误．

MySolution sol;

std::vector<std::vector<std::string>> Solution::get_next_move(const json &response, bool white) {
    JsonMove next_move;
    Board board;
    Moves moves;
    MySolution::get_board(response, board);
    sol.get_moves(board, moves, white);
    // TODO 初始化随机数种子
    // srand(time(NULL));
    int random_idx = std::rand() % moves.size();
    Move move = moves[random_idx];
    MySolution::transfer_move(move, next_move);

    return next_move;
}

