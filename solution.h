#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>

using namespace std;
using json = nlohmann::json;

// 定义一个变体来表示可能的值
using Value = variant<int, vector<string>>; 

// 定义根据需求的字典类型
using Dict = unordered_map<string, Value>; 

// 定义列表类型
using DataList = vector<Dict>;

// 声明 Solution 类
class Solution {
public:
    // 将 get_next_move 声明为公共成员函数
    vector<vector<string>> get_next_move(const json &board, bool white);
};

#endif // SOLUTION_H
