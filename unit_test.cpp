#include "unit_test.h"
#include "my_sol.h"

void test1(MySolution &sol) {
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        1,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    BoardFlag flags; 
    Moves moves;
    Moves expect_moves = {{48, 80, 82, 114}, {48, 80, 112}};
    Move move = {48};
    sol.get_eat_moves(board, flags, moves, move, 64, 16, true, false);
    assert (moves == expect_moves);
}

void test2(MySolution &sol) {
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        1,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    BoardFlag flags; 
    Moves moves;
    Moves expect_moves = {{48, 80, 82, 84, 116}, {48, 80, 82, 114}, {48, 80, 112}};
    Move move = {48};
    sol.get_eat_moves(board, flags, moves, move, 64, 16, true, false);
    assert (moves == expect_moves);
}

void test3(MySolution &sol) {
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    BoardFlag flags; 
    Moves moves;
    Moves expect_moves = {{50, 82, 80, 112}, {50, 82, 84, 116}, {50, 82, 114}};
    Move move = {50};
    sol.get_eat_moves(board, flags, moves, move, 66, 16, true, false);
    assert (moves == expect_moves);
}

void test4(MySolution &sol) {
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {{50, 49}, {50, 51}, {50, 82, 80, 112}, {50, 82, 84, 116}};
    sol.get_press_moves(board, moves, 50);
    assert (moves == expect_moves);
}

void test5(MySolution &sol) {
    Board board = {
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,0,1,0,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,3,1,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {{66, 65}, {66, 34, 32, 0}, {66, 34, 36, 4}};
    sol.get_press_moves(board, moves, 66);
    assert (moves == expect_moves);
}

void test6(MySolution &sol) {
    Board board = {
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,0,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,1,3,1,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,0,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {{66, 64}, {66, 68}, {66, 34}};
    sol.get_press_moves(board, moves, 66);
    assert (moves == expect_moves);
}

void test7(MySolution &sol) {
    Board board = {
        4,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
        {0, 16}, {0, 32}, {0, 48}, {0, 64}};
    sol.get_press_moves(board, moves, 0);
    assert (moves == expect_moves);
}

void test8(MySolution &sol) {
    Board board = {
        4,1,0,1,0,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {
        {0, 2, 4, 7}, {0, 2, 5, 7}
    };
    sol.get_press_moves(board, moves, 0);
    assert (moves == expect_moves);
}

void test9(MySolution &sol) {
    Board board = {
        4,0,1,0,0,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {
        {0, 1}, {0, 3, 7}, {0, 4, 7}, {0, 5, 7}
    };
    sol.get_press_moves(board, moves, 0);
    assert (moves == expect_moves);
}

void test10(MySolution &sol) {
    Board board = {
        3,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        4,0,1,0,1,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,1,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,1,0,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,0,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };

    Moves moves;
    Moves expect_moves = {
        {16, 17}, {16, 19, 21, 23, 55, 53, 50},{16, 19, 21, 23, 55, 53, 49},
        {16, 19, 21, 23, 55, 52, 50},{16, 19, 21, 23, 55, 52, 49}
    };
    sol.get_press_moves(board, moves, 16);
    assert (moves == expect_moves);
}

void test11(MySolution &sol)
{
    Move move = {0, 1};
    JsonMove json_move;
    JsonMove expect_move = {{"a", "8"}, {"b", "8"}};
    MySolution::transfer_move(move, json_move);
    assert (json_move == expect_move);
}

void run_test()
{
    MySolution sol;
    test1(sol);
    test2(sol);
    test3(sol);
    test4(sol);
    test5(sol);
    test6(sol);
    test7(sol);
    test8(sol);
    test9(sol);
    test10(sol);
    test11(sol);
}