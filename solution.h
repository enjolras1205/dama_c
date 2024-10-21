#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "my_sol.h"

using json = nlohmann::json;

class Solution {
public:
    static std::vector<std::vector<std::string>> get_next_move(const json &response, bool white, MySolution &sol);
};
