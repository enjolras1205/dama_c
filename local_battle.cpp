#include "local_battle.h"
#include "my_sol.h"

void get_board_result(const Board &board, int &white_point, int &black_point)
{
    int idx = 0;
    while (!(idx & 0x88)) {
        while (!(idx & 0x88)) {
            auto chess = board[idx];
            switch (chess)
            {
            case white_soldier: {
                white_point += 1;
                break;
            }
            case white_king: {
                white_point += 2;
                break;
            }
            case black_soldier: {
                black_point += 1;
                break;
            }
            case black_king: {
                black_point += 2;
                break;
            }
            default:
                break;
            }
            idx += 1;
        }
        idx += BOARD_BOUND_SIZE;
    }
}

int judge(int white_point, int black_point)
{
  if (white_point <= 1 && black_point > 1) {
    return black_win;
  }
  if (black_point <= 1 && white_point > 1) {
    return white_win;
  }
  return gaming;
}

void local_battle()
{
    MySolution sol{1, 4};
    MySolution sol2{2, 8};
    Move move;
    MoveOps ops;

    int white_win_count = 0;
    int black_win_count = 0;
    int draw_count = 0;
    for (int i = 0; i < 10; ++i) {
      Board board = {
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
          1,0,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
          0,1,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
          3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
      };
      for (int j = 0; true; ++j) {
        MySolution::print_board(board);
        move = sol.get_best_move(board, true);
        sol.do_move(board, move, ops, true);
        MySolution::print_board(board);
        move = sol2.get_best_move(board, false);
        sol.do_move(board, move, ops, false);
        MySolution::print_board(board);
        int white_point = 0;
        int black_point = 0;
        get_board_result(board, white_point, black_point);
        int result = judge(white_point, black_point);
        if (result == white_win) {
          white_win_count += 1;
          break;
        } else if (result == black_win) {
          black_win_count += 1;
          break;
        } else if (j >= 60) {
          if (black_point < white_point) {
            white_win_count += 1;
          } else if(black_point > white_point) {
            black_win_count += 1;
          } else {
            draw_count += 1;
          }
          break;
        }
      }
    }

    std::cout << "white_win_count:" << white_win_count << std::endl;
    std::cout << "black_win_count:" << black_win_count << std::endl;
    std::cout << "draw_count:" << draw_count << std::endl;
}