#include "solution.h"

std::vector<std::vector<std::string>> Solution::get_next_move(const json &response, bool white) {
    std::vector<std::vector<std::string>> next_move;

    if (white) {
        next_move.push_back({"a", "6"});
        next_move.push_back({"a", "5"});
    } else {
        next_move.push_back({"a", "3"});
        next_move.push_back({"a", "4"});
    }

    return next_move;
}

