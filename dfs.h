#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 8
#define WHITE 1
#define BLACK 3
#define EMPTY 0

typedef struct {
    int x, y;
} Move;

typedef struct {
    Move* moves;
    size_t size;
} MoveList;

MoveList dfs(int chess[SIZE][SIZE], int i, int j, bool lady, int color, int n, int m, int pre_x, int pre_y);
MoveList explore_direction(int chess[SIZE][SIZE], int i, int j, int qx, int qy, bool lady, int color, int n, int m, MoveList best_paths, int nx, int ny);
MoveList explore_capture(int chess[SIZE][SIZE], int i, int j, int qx, int qy, bool lady, int color, int n, int m, MoveList best_paths, int nx, int ny, int rx, int ry);
MoveList move_list(int turkish_flag[SIZE][SIZE], bool white);
MoveList get_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int chess_type);
void explore_free_list(int turkish_flag[SIZE][SIZE], int i, int j, int cx, int cy, int dx, int dy, MoveList* res, int n, int m);
void free_move_list(MoveList* moves);
bool is_valid_position(int x, int y);

bool is_valid_position(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

MoveList dfs(int chess[SIZE][SIZE], int i, int j, bool lady, int color, int n, int m, int pre_x, int pre_y) {
    MoveList best_paths;
    best_paths.size = 0;
    best_paths.moves = malloc(100 * sizeof(Move)); 

    if (lady) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; 
                int nx = i + dx;
                int ny = j + dy;
                
                best_paths = explore_direction(chess, i, j, dx, dy, lady, color, n, m, best_paths, nx, ny);
            }
        }
    } else {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; 
                if ((color == WHITE && dx == -1) || (color == BLACK && dx == 1)) continue;

                int nx = i + dx;
                int ny = j + dy;

                if (is_valid_position(nx, ny) &&
                    (((chess[nx][ny] == BLACK && chess[i][j] == WHITE) || 
                      (chess[nx][ny] == WHITE && chess[i][j] == BLACK)))) {

                    int rx = nx + dx;
                    int ry = ny + dy;

                    if (is_valid_position(rx, ry) && chess[rx][ry] == EMPTY) {
                        int temp = chess[nx][ny];
                        chess[nx][ny] = EMPTY;  
                        chess[rx][ry] = chess[i][j];  
                        chess[i][j] = EMPTY;  

                        MoveList current_paths = dfs(chess, rx, ry, lady, color, n, m, dx, dy);
                        for (size_t k = 0; k < current_paths.size; k++) {
                            best_paths.moves[best_paths.size++] = (Move){i, j}; 
                        }

                        chess[i][j] = chess[rx][ry];
                        chess[nx][ny] = temp;
                        chess[rx][ry] = EMPTY;
                    }
                }
            }
        }
    }

    if (best_paths.size == 0) {
        best_paths.size = 1; 
        best_paths.moves[0] = (Move){i, j};
    }

    return best_paths;
}

MoveList explore_direction(int chess[SIZE][SIZE], int i, int j, int qx, int qy, bool lady, int color, int n, int m, MoveList best_paths, int nx, int ny) {
    if (is_valid_position(nx, ny)) {
        if ((color == WHITE && (chess[nx][ny] == BLACK_PIECE || chess[nx][ny] == BLACK_PIECE + 1)) ||
            (color == BLACK && (chess[nx][ny] == WHITE_PIECE || chess[nx][ny] == WHITE_PIECE + 1))) {
            return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, nx + qx, ny + qy);
        } else if (chess[nx][ny] == EMPTY) {
            return explore_direction(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx + qx, ny + qy);
        }
    }
    return best_paths;
}

MoveList explore_capture(int chess[SIZE][SIZE], int i, int j, int qx, int qy, bool lady, int color, int n, int m, MoveList best_paths, int nx, int ny, int rx, int ry) {
    if (is_valid_position(rx, ry) && chess[rx][ry] == EMPTY) {
        int temp = chess[nx][ny];
        chess[nx][ny] = EMPTY; 
        chess[rx][ry] = chess[i][j];  
        chess[i][j] = EMPTY;  
        
        MoveList current_paths = dfs(chess, rx, ry, lady, color, n, m, qx, qy);
        
        chess[i][j] = chess[rx][ry]; 
        chess[rx][ry] = EMPTY;  
        chess[nx][ny] = temp;  

        for (size_t k = 0; k < current_paths.size; k++) {
            best_paths.moves[best_paths.size++] = (Move){i, j}; 
        }

        return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, rx + qx, ry + qy);
    }
    return best_paths;
}

MoveList move_list(int turkish_flag[SIZE][SIZE], bool white) {
    MoveList overall_best_paths;
    overall_best_paths.size = 0;
    overall_best_paths.moves = malloc(100 * sizeof(Move)); 

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (white && (turkish_flag[i][j] == WHITE || turkish_flag[i][j] == WHITE + 1)) {
                MoveList paths = dfs(turkish_flag, i, j, false, WHITE, SIZE, SIZE, 0, 0);
                for (size_t k = 0; k < paths.size; k++) {
                    overall_best_paths.moves[overall_best_paths.size++] = paths.moves[k];
                }
                free_move_list(&paths);
            } else if (!white && (turkish_flag[i][j] == BLACK || turkish_flag[i][j] == BLACK + 1)) {
                MoveList paths = dfs(turkish_flag, i, j, false, BLACK, SIZE, SIZE, 0, 0);
                for (size_t k = 0; k < paths.size; k++) {
                    overall_best_paths.moves[overall_best_paths.size++] = paths.moves[k];
                }
                free_move_list(&paths);
            }
        }
    }
    
    return overall_best_paths;
}

void free_move_list(MoveList* moves) {
    free(moves->moves);
}
