#include <nlohmann/json.hpp>
#include <vector>
#include <string>

using json = nlohmann::json;
// https://www.xqbase.com/computer/struct_0x88.htm
const uint16_t BOARD_SIZE = 128;
// >>> 0x88 & 0
// 0
// >>> 0x88 & 15
// 8
// >>> 0x88 & 16
// 0
// 一次移动。[idx1, idx2, ... idxN]
// H6 -> H4 -> F4 ...
using Move = std::vector<int>;

// 1 白兵
// 2 白王
// 3 黑兵
// 4 黑王
//   A B C D E F G H
// 8 0 0 0 0 0 0 0 0
// 7 1 1 1 1 1 1 1 1
// 6 1 1 1 1 1 1 1 1
// 5 0 0 0 0 0 0 0 0
// 4 0 0 0 0 0 0 0 0
// 3 3 3 3 3 3 3 3 3 
// 2 3 3 3 3 3 3 3 3 
// 1 0 0 0 0 0 0 0 0 
//   A B C D E F G H
using Board = std::array<int, BOARD_SIZE>;

class Solution {
public:
    static std::vector<std::vector<std::string>> get_next_move(const json &response, bool white);
    static void get_board(const json &response, bool white, Board &board);
};
