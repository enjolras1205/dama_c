#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE_PIECE 1
#define BLACK_PIECE 3
#define EMPTY 0

typedef struct {
    int x, y;
} Move;

typedef struct {
    Move* moves;
    size_t size;
} MoveList;

// Function declarations
void explore_direction(int chess[8][8], int i, int j, int qx, int qy, int color, int n, int m, MoveList* best_paths, int nx, int ny);
void explore_capture(int chess[8][8], int i, int j, int qx, int qy, int color, int n, int m, MoveList* best_paths, int nx, int ny, int rx, int ry);
void dfs(int chess[8][8], int i, int j, int lady, int color, int n, int m, int pre_x, int pre_y, MoveList* best_paths);
MoveList move_list(int turkish_flag[8][8], int white);
MoveList get_free_list(int turkish_flag[8][8], int i, int j, int chess_type);
void explore_free_list(int turkish_flag[8][8], int i, int j, int cx, int cy, int dx, int dy, MoveList* res, int n, int m);

// 函数实现
void explore_direction(int chess[8][8], int i, int j, int qx, int qy, int color, int n, int m, MoveList* best_paths, int nx, int ny) {
    if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
        if ((color == 1 && (chess[nx][ny] == 3 || chess[nx][ny] == 4)) || 
            (color == 3 && (chess[nx][ny] == 1 || chess[nx][ny] == 2))) {
            explore_capture(chess, i, j, qx, qy, color, n, m, best_paths, nx, ny, nx + qx, ny + qy);
        } else if (chess[nx][ny] == 0) {
            explore_direction(chess, i, j, qx, qy, color, n, m, best_paths, nx + qx, ny + qy);
        }
    }
}

void explore_capture(int chess[8][8], int i, int j, int qx, int qy, int color, int n, int m, MoveList* best_paths, int nx, int ny, int rx, int ry) {
    if (rx >= 0 && rx < n && ry >= 0 && ry < m && chess[rx][ry] == 0) {
        int temp = chess[nx][ny];
        chess[nx][ny] = 0;  // 移除对方棋子
        chess[rx][ry] = chess[i][j];  // 移动自己的棋子
        chess[i][j] = 0;  // 清空当前位置
        MoveList current_paths;
        current_paths.size = 0;
        current_paths.moves = NULL;
        // Perform DFS after the move
        dfs(chess, rx, ry, 1, color, n, m, qx, qy, &current_paths);
        chess[i][j] = chess[rx][ry];  // 恢复自己的棋子
        chess[rx][ry] = 0;  // 回溯，恢复原始状态
        chess[nx][ny] = temp;  // 恢复对方棋子

        for (size_t k = 0; k < current_paths.size; k++) {
            best_paths->moves[best_paths->size].x = i;
            best_paths->moves[best_paths->size].y = j;
            best_paths->size++;
        }
        explore_capture(chess, i, j, qx, qy, color, n, m, best_paths, nx, ny, rx + qx, ry + qy);
    }
}

void dfs(int chess[8][8], int i, int j, int lady, int color, int n, int m, int pre_x, int pre_y, MoveList* best_paths) {
    if (lady) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (abs(dx) == abs(dy) && (dx != 0 || dy != 0)) {
                    continue; // Skip diagonals
                }
                int nx = i + dx;
                int ny = j + dy;
                explore_direction(chess, i, j, dx, dy, color, n, m, best_paths, nx, ny);
            }
        }
    }
}

MoveList move_list(int turkish_flag[8][8], int white) {
    MoveList best_paths_overall;
    best_paths_overall.size = 0;
    best_paths_overall.moves = malloc(100 * sizeof(Move));  // 预分配内存，根据需要调整大小

    int n = 8; // Chessboard Height
    int m = 8; // Chessboard Width
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Check for pieces based on color
            if (white && (turkish_flag[i][j] == 1 || turkish_flag[i][j] == 2)) {
                // White piece logic
                // Call DFS here
            } else if (!white && (turkish_flag[i][j] == 3 || turkish_flag[i][j] == 4)) {
                // Black piece logic
                // Call DFS here
            }
        }
    }

    if (best_paths_overall.size == 0) {
        // Implement logic for when there are no best paths
    }

    return best_paths_overall;
}

MoveList get_free_list(int turkish_flag[8][8], int i, int j, int chess_type) {
    MoveList res;
    res.size = 0;
    res.moves = malloc(100 * sizeof(Move)); // Preallocate space for moves
    int n = 8;
    int m = 8;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = i + dx;
            int ny = j + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && turkish_flag[nx][ny] == EMPTY) {
                res.moves[res.size].x = nx;
                res.moves[res.size].y = ny;
                res.size++;
            }
        }
    }

    return res;
}

void explore_free_list(int turkish_flag[8][8], int i, int j, int cx, int cy, int dx, int dy, MoveList* res, int n, int m) {
    int nx = cx + dx;
    int ny = cy + dy;
    if (nx >= 0 && nx < n && ny >= 0 && ny < m && turkish_flag[nx][ny] == EMPTY) {
        res->moves[res->size].x = nx;
        res->moves[res->size].y = ny;
        res->size++;
        explore_free_list(turkish_flag, i, j, nx, ny, dx, dy, res, n, m);
    }
}
