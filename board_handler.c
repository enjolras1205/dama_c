#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board_handler.h"

void convert_to_matrix(const char board_data[][2], int board[SIZE][SIZE], int size) {
    memset(board, 0, sizeof(int) * SIZE * SIZE); 
    for (int i = 0; i < size; i++) {
        int x = board_data[i][0] - '1'; 
        int y = board_data[i][1] - 'a';  
        board[x][y] = 1; 
    }
}

char* get_next_move(int* board[8][8], int white) {
    static char* res[2];
    // // 定义整数到字符的映射
    char reflection_y_to_x[SIZE];
    char reflection_x_to_y[SIZE];

    // for (int i = 0; i < SIZE; i++) {
    //     reflection_y_to_x[i] = 'a' + i; // y: 0 -> 'a', ...
    //     reflection_x_to_y[i] = '8' - i; // x: 0 -> '8', ...
    // }

    // // 转换为整数矩阵
    // int chess[SIZE][SIZE] = {0};
    // for (int i = 0; i < SIZE; i++) {
    //     int y = board[i][0] - 'a';
    //     int x = '8' - board[i][1]; // 转换字符到数字
    //     chess[x][y] = 1; // 假设所有的棋子值为 1
    // }

    // 获取可能的移动
    int** moves = move_list(board, white);
    if (moves == NULL) {
        printf("No moves available.\n");
        return NULL;
    }

    // 随机选择一个移动
    int move_count = 0;
    while (moves[move_count] != NULL) {
        move_count++;
    }
    int random_index = rand() % move_count;
    res[0] = reflection_y_to_x[moves[random_index][1]];
    res[1] = reflection_x_to_y[moves[random_index][0]];
    
    // 打印选中的移动
    printf("Next move: %c%c\n", reflection_y_to_x[moves[random_index][1]], reflection_x_to_y[moves[random_index][0]]);
    
    // 释放 moves 的内存
    for (int i = 0; i < move_count; i++) {
        free(moves[i]);
    }
    free(moves);
    return res;
}

// 释放 allocated memory for best_paths
void free_best_paths(int **best_paths) {
    for (int i = 0; best_paths[i] != NULL; i++) {
        free(best_paths[i]);
    }
    free(best_paths);
}
