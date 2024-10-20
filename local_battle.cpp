#include "local_battle.h"
#include "my_sol.h"
#include "my_sol_v1.h"
using namespace std::chrono;

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
    MySolutionV1 sol{8};
    MoveOps ops;

    bool v1_use_white = true;
    int white_win_count = 0;
    int black_win_count = 0;
    int draw_count = 0;
    uint64_t sol1_ms = 0;
    uint64_t sol2_ms = 0;
    int battle_level = 1;
    std::string msg = "sort: v1_8 vs v2_9";
    std::cout<<"battle_level:"<<battle_level<<std::endl;
    std::cout<<"msg:"<<msg<<std::endl;
    for (int i = 0; i < 10; ++i) {
      MySolution sol2{12, 5000, 9};
      Board board = {
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
          1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
          3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
          3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
          0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
      };
      for (int j = 0; true; ++j) {
        MySolution::print_board(board);
        
        // old
        auto ms1 = timeSinceEpochMillisec();
        auto move1 = sol.get_best_move(board, v1_use_white);
        auto ms2 = timeSinceEpochMillisec();
        sol1_ms += ms2 - ms1;
        sol.do_move(board, move1, ops, v1_use_white);
        json xx1 = {{"move1", move1}};
        std::cout << xx1.dump() << std::endl;

        MySolution::print_board(board);

        // new
        auto ms3 = timeSinceEpochMillisec();
        auto move2 = sol2.get_best_move(board, !v1_use_white);
        auto ms4 = timeSinceEpochMillisec();
        sol2_ms += ms4 - ms3;
        sol2.do_move(board, move2, ops, !v1_use_white);
        json xx2 = {{"move2", move2}};
        std::cout << xx2.dump() << std::endl;

        MySolution::print_board(board);
        int board_point_1 = MySolutionV1::calc_board(board, v1_use_white);
        int board_point_2 = MySolution::calc_board(board, !v1_use_white);
        std::cout << "sol1_ms:" << sol1_ms << " current_ms:" << (ms2 - ms1) << " board_point:" << board_point_1 << std::endl;
        std::cout << "sol2_ms:" << sol2_ms << " current_ms:" << (ms4 - ms3) << " board_point:" << board_point_2 << std::endl;
        sol2.print_status();
        int white_point = 0;
        int black_point = 0;
        get_board_result(board, white_point, black_point);
        int result = judge(white_point, black_point);
        if (result == white_win) {
          white_win_count += 1;
          std::cout<<"result:white_win_a"<<std::endl;
          break;
        } else if (result == black_win) {
          std::cout<<"result:black_win_a"<<std::endl;
          black_win_count += 1;
          break;
        } else if (j >= 60) {
          if (black_point < white_point) {
            std::cout<<"result:white_win_b"<<std::endl;
            white_win_count += 1;
          } else if(black_point > white_point) {
            std::cout<<"result:black_win_b"<<std::endl;
            black_win_count += 1;
          } else {
            std::cout<<"result:draw"<<std::endl;
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