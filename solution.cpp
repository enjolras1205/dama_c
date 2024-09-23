#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <algorithm> 
#include <random>
#include "dfs.h"
#include "solution.h"

using namespace std;

using Board = vector<vector<int>>;  // 8x8 游戏棋盘

void from_json(const json& j, Value& value) {
    if (j.is_number()) {
        value = j.get<int>();
    } else if (j.is_array()) {
        value = j.get<std::vector<std::string>>();
    } else {
        throw std::invalid_argument("Invalid type for variant conversion");
    }
}

void to_json(json& j, const Value& value) {
    std::visit([&j](const auto& v) {
        j = v; // 直接赋值，将 v 转换为 JSON 类型
    }, value);
}


vector<vector<string>> Solution::get_next_move(const json &response, bool white) {
    // 初始化映射
    unordered_map<int, string> reflection_y_to_x;
    for (int i = 0; i < 8; i++) {
        reflection_y_to_x[i] = "a" + i; // a, b, c, d, e, f, g, h
    }

    unordered_map<int, string> reflection_x_to_y;
    for (int i = 0; i < 8; i++) {
        reflection_x_to_y[i] = "8" - i; // 8, 7, 6, 5, 4, 3, 2, 1
    }

    unordered_map<string, int> reflection_y_to_index;
    for (const auto &[k, v] : reflection_y_to_x) {
        reflection_y_to_index[v] = k; 
    }

    unordered_map<string, int> reflection_x_to_index;
    for (const auto &[k, v] : reflection_x_to_y) {
        reflection_x_to_index[v] = k; 
    }
    DataList board = response["board"].get<DataList>();

    // 转换输入数据为棋盘矩阵
    Board mat(8, vector<int>(8, 0)); // 8x8 初始化为0
    for (const auto &item : board) {
        vector<string> index = get<vector<string>>((item.at("index")));
        int value = get<int>(item.at("value"));

        int y = reflection_y_to_index[index[0]]; // y index
        int x = reflection_x_to_index[index[1]]; // x index
        mat[x][y] = value; // 设置矩阵中的值
    }

    vector<vector<pair<int, int>>> moves = move_list(mat, white); 

    // 随机选择一步
    srand(static_cast<unsigned>(time(0))); // 随机种子
    shuffle(moves.begin(), moves.end(), default_random_engine(random_device()())); // 将所有移动随机打乱

    // 选择第一步
    vector<pair<int, int>> move = moves.front(); // 获取随机步
    vector<vector<string>> queue;

    for (const auto &step : move) {
        vector<string> convert_step = {
            reflection_y_to_x[step.second], 
            reflection_x_to_y[step.first]  
        };
        queue.push_back(convert_step);
    }

    return queue;
};

