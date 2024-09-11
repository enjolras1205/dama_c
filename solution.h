#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 8

// 数据结构表示棋盘的每个位置
typedef struct {
    char index[3]; // e.g. "a8"
    int value;     // 棋子值
} BoardItem;

// 映射结构
char* reflection_y_to_x[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
char* reflection_x_to_y[] = {"8", "7", "6", "5", "4", "3", "2", "1"};

// 辅助函数用于将字符索引转换为数字索引
int reflection_y_to_index(const char *index) {
    return index[0] - 'a'; // 'a' 对应0, 'b' 对应1, ..., 'h' 对应7
}

int reflection_x_to_index(const char *index) {
    return '8' - index[0]; // '8' 对应0, '7' 对应1, ..., '1' 对应7
}

// 初始化棋盘
void convert_to_matrix(BoardItem data[], int size, int board[SIZE][SIZE]) {
    // 初始化棋盘
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0; // 默认值
        }
    }

    for (int i = 0; i < size; ++i) {
        int x = reflection_x_to_index(data[i].index[1]);
        int y = reflection_y_to_index(data[i].index); // 取出字母部分
        board[x][y] = data[i].value;
    }
}

// 随机选择移动，模拟 DFS 动作
void get_next_move(BoardItem board[], int size, int white) {
    int chess_board[SIZE][SIZE];
    convert_to_matrix(board, size, chess_board);
    
   
}

int main() {
    // 示例棋盘数据
    BoardItem board[] = {
        {"a8", 0}, {"b8", 1}, {"c8", 2}, 
        {"d8", 0}, {"e8", 0}, {"f8", 0}, 
        {"g8", 0}, {"h8", 0}, 
        {"a7", 1}, {"b7", 2} // 添加更多棋子
    };

    int board_size = sizeof(board) / sizeof(board[0]);
    int white = 1; // 代表白方

    // 获取下一步移动
    get_next_move(board, board_size, white);

    return 0;
}
