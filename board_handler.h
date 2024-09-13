#ifndef BOARD_HANDLER_H    
#define BOARD_HANDLER_H 

#define SIZE 8   


void convert_to_matrix(const char board_data[][2], int board[SIZE][SIZE], int size);
char* get_next_move(int board[SIZE][SIZE], int white);
void free_best_paths(int **best_paths);
int** move_list(int* chess[SIZE][SIZE], int white); 

#endif 
