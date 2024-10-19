#include <random>
#include <time.h>
#include <cstdlib>
#include "solution.h"
#include "my_sol.h"

MySolution sol;

JsonMove get_random_move(const json &response, bool white) {
    JsonMove next_move;
    Board board;
    Moves moves;
    MySolution::get_board(response, board);
    sol.get_moves(board, moves, white);
    int random_idx = std::rand() % moves.size();
    Move move = moves[random_idx];
    MySolution::transfer_move(move, next_move);

    return next_move;
}

std::vector<std::vector<std::string>> Solution::get_next_move(const json &response, bool white) {
    JsonMove next_move;
    Board board;
    Moves moves;
    int_fast64_t val = 0;
    MySolution::get_board(response, board, val);
    Move move = sol.get_best_move(board, white, val);
    MySolution::transfer_move(move, next_move);

    return next_move;
}

