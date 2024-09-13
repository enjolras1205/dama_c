#ifndef DFS_H  
#define DFS_H
#define SIZE 8
// 函数声明
int** explore_direction(int chess[SIZE][SIZE], int i, int j, int qx, int qy, int lady, const char* color, int n, int m, int** best_paths, int nx, int ny);
int** explore_capture(int chess[SIZE][SIZE], int i, int j, int qx, int qy, int lady, const char* color, int n, int m, int** best_paths, int nx, int ny, int rx, int ry);
int** dfs(int chess[SIZE][SIZE], int i, int j, int lady, const char* color, int n, int m, int pre_x, int pre_y);
int** move_list(int turkish_flag[SIZE][SIZE], int white);
int** get_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int chess_type, int n, int m);
int** explore_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int cx, int cy, int dx, int dy, int** res, int n, int m);
int** pairwise();

#endif // DFS_H
