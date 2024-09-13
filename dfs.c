#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 8 // 假设棋盘的大小为 8


// 辅助函数：初始化 2D 数组
int** init_2d_array(int rows, int cols) {
    int** arr = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        arr[i] = (int*)malloc(cols * sizeof(int));
    }
    return arr;
}

// 探索方向
int** explore_direction(int chess[SIZE][SIZE], int i, int j, int qx, int qy, int lady, const char* color, int n, int m, int** best_paths, int nx, int ny) {
    if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
        if ((strcmp(color, "white") == 0 && (chess[nx][ny] == 3 || chess[nx][ny] == 4)) ||
            (strcmp(color, "black") == 0 && (chess[nx][ny] == 1 || chess[nx][ny] == 2))) {
            return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, nx + qx, ny + qy);
        } else if (chess[nx][ny] == 0) {
            return explore_direction(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx + qx, ny + qy);
        }
    }
    return best_paths;
}

// 探索捕捉
int** explore_capture(int chess[SIZE][SIZE], int i, int j, int qx, int qy, int lady, const char* color, int n, int m, int** best_paths, int nx, int ny, int rx, int ry) {
    if (rx >= 0 && rx < n && ry >= 0 && ry < m && chess[rx][ry] == 0) {
        int temp = chess[nx][ny];
        chess[nx][ny] = 0;  // 移除对方棋子
        chess[rx][ry] = chess[i][j];  // 移动自己的棋子
        chess[i][j] = 0;  // 清空当前棋子的位置
        
        int** current_paths = dfs(chess, rx, ry, lady, color, n, m, qx, qy);
        
        chess[i][j] = chess[rx][ry];  // 恢复自己的棋子
        chess[rx][ry] = 0;  // 回溯，恢复原始状态
        chess[nx][ny] = temp;  // 恢复对方棋子
        
        // 处理路径
        for (int p = 0; current_paths[p] != NULL; p++) {
            best_paths = append_path(best_paths, i, j, current_paths[p]);
        }
        
        // 继续方向捕捉
        return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, rx + qx, ry + qy);
    }
    return best_paths;
}

// 深度优先搜索
int** dfs(int chess[SIZE][SIZE], int i, int j, int lady, const char* color, int n, int m, int pre_x, int pre_y) {
    int** best_paths = init_2d_array(0, 2);  // 初始化最佳路径

    if (lady) {
        int directions[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
        for (int k = 0; k < 4; k++) {
            int qx = directions[k][0];
            int qy = directions[k][1];
            if (qx * -1 == pre_x && qy * -1 == pre_y) continue;
            
            int nx = i + qx;
            int ny = j + qy;
            best_paths = explore_direction(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny);
        }
    } else {
        int directions[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
        for (int k = 0; k < 4; k++) {
            int qx = directions[k][0];
            int qy = directions[k][1];
            if ((strcmp(color, "white") == 0 && qx == -1) || (strcmp(color, "black") == 0 && qx == 1)) continue;

            int nx = i + qx;
            int ny = j + qy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                (((chess[nx][ny] == 3 || chess[nx][ny] == 4) && chess[i][j] == 1) ||
                ((chess[nx][ny] == 1 || chess[nx][ny] == 2) && chess[i][j] == 3))) {
                
                int rx = nx + qx;
                int ry = ny + qy;
                if (rx >= 0 && rx < n && ry >= 0 && ry < m && chess[rx][ry] == 0) {
                    int temp = chess[nx][ny];
                    chess[nx][ny] = 0;
                    chess[rx][ry] = chess[i][j];
                    chess[i][j] = 0;
                    int** current_paths = dfs(chess, rx, ry, lady, color, n, m, qx, qy);
                    
                    // 恢复棋盘
                    chess[i][j] = chess[rx][ry];
                    chess[nx][ny] = temp;
                    chess[rx][ry] = 0;

                    // 处理路径
                    for (int p = 0; current_paths[p] != NULL; p++) {
                        best_paths = append_path(best_paths, i, j, current_paths[p]);
                    }
                }
            }
        }
    }

    // 如果没有路径则返回当前路径
    if (!best_paths || best_paths[0] == NULL) {
        best_paths = init_2d_array(1, 2);
        best_paths[0][0] = i;
        best_paths[0][1] = j;
    }
    
    return best_paths;
}

// 移动列表
int** move_list(int turkish_flag[SIZE][SIZE], int white) {
    int n = SIZE; // 行数
    int m = SIZE; // 列数
    int** best_paths_overall = init_2d_array(0, 2);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (white) {
                if (turkish_flag[i][j] == 1 || turkish_flag[i][j] == 2) {
                    int** paths;
                    if (turkish_flag[i][j] == 1) {
                        paths = dfs(turkish_flag, i, j, 0, "white", n, m, 0, 0);
                    } else {
                        paths = dfs(turkish_flag, i, j, 1, "white", n, m, 0, 0);
                    }
                    
                    for (int p = 0; paths[p] != NULL; p++) {
                        if (!best_paths_overall || strlen(paths[p]) > strlen(best_paths_overall[0])) {
                            best_paths_overall = (int**) realloc(best_paths_overall, sizeof(int*) * (p + 1)); // Updating overall paths
                            best_paths_overall[p] = paths[p];
                        }
                    }
                }
            }
            else {
                if (turkish_flag[i][j] == 3 || turkish_flag[i][j] == 4) {
                    int** paths;
                    if (turkish_flag[i][j] == 3) {
                        paths = dfs(turkish_flag, i, j, 0, "black", n, m, 0, 0);
                    } else {
                        paths = dfs(turkish_flag, i, j, 1, "black", n, m, 0, 0);
                    }
                    
                    for (int p = 0; paths[p] != NULL; p++) {
                        if (!best_paths_overall || strlen(paths[p]) > strlen(best_paths_overall[0])) {
                            best_paths_overall = (int**) realloc(best_paths_overall, sizeof(int*) * (p + 1)); // Updating overall paths
                            best_paths_overall[p] = paths[p];
                        }
                    }
                }
            }
        }
    }
    
    return best_paths_overall;
}

// 自由移动列表
int** get_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int chess_type, int n, int m) {
    int** res = init_2d_array(0, 2);
    int directions[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    
    for (int k = 0; k < 4; k++) {
        int dx = directions[k][0];
        int dy = directions[k][1];
        
        if (chess_type == 1 || chess_type == 3) {
            // 处理不同棋子的逻辑
            // ...
        }
        // 继续实现逻辑
    }
    return res;
}

// 自由移动列表递归
int** explore_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int cx, int cy, int dx, int dy, int** res, int n, int m) {
    int nx = cx + dx;
    int ny = cy + dy;
    
    if (nx >= 0 && nx < n && ny >= 0 && ny < m && turkish_flag[nx][ny] == 0) {
        // 将当前位置添加到结果中
        // ...
        return explore_free_list(turkish_flag, i, j, nx, ny, dx, dy, res, n, m);  // 递归调用
    }
    else {
        return res;
    }
}

// 主程序（测试用）
int main() {
    // 可以在这里写一些测试代码
    return 0;
}
