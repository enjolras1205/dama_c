#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "dfs.h"

#define SIZE 8
#define EMPTY 0
#define WHITE 1
#define BLACK 3

// 数据结构
typedef struct {
    int x, y;
} Move;

typedef struct {
    Move* moves;
    size_t size;
} MoveList;

// 映射数组
const char *reflection_y_to_x[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
const char *reflection_x_to_y[] = {"8", "7", "6", "5", "4", "3", "2", "1"};

int reflection_x_to_index(const char *index) {
    return index[0] - '8'; 
}

int reflection_y_to_index(const char *index) {
    return index[0] - 'a'; 
}

int *convert_to_matrix(int board_data[][2], int data_size, int board[SIZE][SIZE]) {
    // 初始化棋盘
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < data_size; ++i) {
        const char *index = board_data[i][0]; 
        int value = board_data[i][1]; 

        int y = reflection_y_to_index(index[0]);
        int x = reflection_x_to_index(index[1]);

        board[x][y] = value; 
    }
    return board;
}

// 选择随机移动
Move random_move(MoveList moves) {
    if (moves.size == 0) {
        return (Move) { -1, -1 }; 
    }

    int random_index = rand() % moves.size; 
    return moves.moves[random_index]; 
}

MoveList get_next_move(int board_data[][2], int data_size, bool white) {
    int board[SIZE][SIZE];
    convert_to_matrix(board_data, data_size, board);

    MoveList moves = move_list(board, white);
    Move move = random_move(moves); 
    
    MoveList result;
    result.size = 0;
    result.moves = malloc(SIZE * sizeof(Move)); 

    if (move.x != -1 && move.y != -1) {
        result.moves[result.size++] = move; 
    }
    
    free(moves.moves);
    return result;
}
