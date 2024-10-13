#ifndef UTIL_H
#define UTIL_H

#include <cassert>
#include <vector>
#include <array>
#include <bitset>
#include <iostream>

constexpr int pos_out_of_board = 0;
constexpr int pos_empty = 1;
constexpr int pos_white = 2;
constexpr int pos_black = 3;

constexpr int empty_chess = 0;
constexpr int white_soldier = 1;
constexpr int white_king = 2;
constexpr int black_soldier = 3;
constexpr int black_king = 4;
constexpr int BOARD_BOUND_SIZE = 8;
constexpr int BOARD_LINE_SIZE = BOARD_BOUND_SIZE * 2;
constexpr int BOARD_SIZE = BOARD_BOUND_SIZE * BOARD_BOUND_SIZE * 2;

// >>> 0x88 & 0
// 0
// >>> 0x88 & 15
// 8
// >>> 0x88 & 16
// 0
// 一次移动。[idx1, idx2, ... idxN]
// H6 -> H4 -> F4 ...
using Move = std::vector<int>;
using MoveDirections = std::vector<int>;

// 1 白兵
// 2 白王
// 3 黑兵
// 4 黑王
//   A B C D E F G H
// 8 0 0 0 0 0 0 0 0
// 7 1 1 1 1 1 1 1 1
// 6 1 1 1 1 1 1 1 1
// 5 0 0 0 0 0 0 0 0
// 4 0 0 0 0 0 0 0 0
// 3 3 3 3 3 3 3 3 3 
// 2 3 3 3 3 3 3 3 3 
// 1 0 0 0 0 0 0 0 0 
//   A B C D E F G H
using Board = std::array<int, BOARD_SIZE>;
using Moves = std::vector<Move>;
using BoardFlag = std::bitset<BOARD_SIZE>;

inline bool is_white_chess(int chess) {
    return chess == white_soldier || chess == white_king;
}

inline bool is_black_chess(int chess) {
    return chess == black_soldier || chess == black_king;
}

inline bool is_my_chess(bool is_white, int chess) {
  if (is_white) {
    return is_white_chess(chess);
  } else {
    return is_black_chess(chess);
  }
}

inline int get_board_pos_type(const Board &board, int idx) {
  if (idx & 0x88) {
    return pos_out_of_board;
  }

  int chess = board[idx];
  if (chess == empty_chess) {
    return pos_empty;
  } else if (!is_white_chess(chess)) {
    return pos_black;
  } else {
    return pos_white;
  }
}

inline bool is_on_board_empty_pos(const Board &board, int idx) {
  return !(idx & 0x88) && board[idx] == empty_chess;
}

inline bool is_white_solider(int chess) {
  return chess == white_soldier;
}

inline bool is_white_king(int chess) {
  return chess == white_king;
}

inline bool is_black_solider(int chess) {
  return chess == black_soldier;
}

inline bool is_black_king(int chess) {
  return chess == black_king;
}

#endif // UTIL_H
