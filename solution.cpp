#include "solution.h"
using namespace std;

std::vector<std::vector<std::string>> Solution::get_next_move(const json &response, bool white) {
    std::vector<std::vector<std::string>> next_move;
    Board board = {-1};
    Solution::get_board(response, white, board);

    if (white) {
        next_move.push_back({"a", "6"});
        next_move.push_back({"a", "5"});
    } else {
        next_move.push_back({"a", "3"});
        next_move.push_back({"a", "4"});
    }

    return next_move;
}

// [{"value": 0, "index": ["a", "8"]}, ...]
void Solution::get_board(const json &response, bool white, Board &board)
{
    auto &board_data = response["board"];
    for (auto &v : board_data) {
        const char a = 'a';
        const string &idx_1 = v["index"][0];
        const string &idx_2 = v["index"][1];
        uint16_t idx = 0;
        idx += (idx_1[0] - a);
        idx += (std::stoi(idx_2) - 1) * 16;
        board[idx] = v["value"];
    }

    return;
}
