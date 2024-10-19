#include "local_battle.h"
#include "my_sol.h"
#include "my_sol_v1.h"
using namespace std::chrono;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

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
    MySolutionV1 sol{4};
    // 新策略有略微优势, 但耗时增加一倍
    MySolution sol2{4};
    Move move;
    MoveOps ops;

    int white_win_count = 0;
    int black_win_count = 0;
    int draw_count = 0;
    uint64_t sol1_ms = 0;
    uint64_t sol2_ms = 0;
    for (int i = 0; i < 100; ++i) {
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
        auto ms1 = timeSinceEpochMillisec();
        move = sol.get_best_move(board, true);
        auto ms2 = timeSinceEpochMillisec();
        sol1_ms += ms2 - ms1;
        sol.do_move(board, move, ops, true);
        MySolution::print_board(board);
        // TODO HASH_KEY
        auto ms3 = timeSinceEpochMillisec();
        move = sol2.get_best_move(board, false, 0);
        auto ms4 = timeSinceEpochMillisec();
        sol2_ms += ms4 - ms3;
        sol.do_move(board, move, ops, false);
        MySolution::print_board(board);
        std::cout << "sol1_ms:" << sol1_ms << " current_ms:" << (ms2 - ms1) << std::endl;
        std::cout << "sol2_ms:" << sol2_ms << " current_ms:" << (ms4 - ms3) << std::endl;
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