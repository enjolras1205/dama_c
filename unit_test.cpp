#include "unit_test.h"
#include "my_sol.h"
#include "my_sol_v1.h"

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

    Moves moves;
    Moves expect_moves = {{48, 80, 82, 114}, {48, 80, 112}};
    Move move = {48};
    sol.get_eat_moves(board, moves, move, 64, 16, true, false);
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

    Moves moves;
    Moves expect_moves = {{48, 80, 82, 84, 116}, {48, 80, 82, 114}, {48, 80, 112}};
    Move move = {48};
    sol.get_eat_moves(board, moves, move, 64, 16, true, false);
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

    Moves moves;
    Moves expect_moves = {{50, 82, 80, 112}, {50, 82, 84, 116}, {50, 82, 114}};
    Move move = {50};
    sol.get_eat_moves(board, moves, move, 66, 16, true, false);
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

void test11()
{
    Move move = {0, 1};
    JsonMove json_move;
    JsonMove expect_move = {{"a", "8"}, {"b", "8"}};
    MySolution::transfer_move(move, json_move);
    assert (json_move == expect_move);
}

void test12(MySolution &sol)
{
    Move wrong_move = Move{67, 51};
    for (auto i = 0; i < 2; ++i) {
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
        Move move = sol.get_best_move(board, false);
        assert (move != wrong_move);
        json xx = {{"move", move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
    }
}

void test13()
{
    // 8层搜索固定返回这个
    // 9层搜索返回 {65,64}
    MySolutionV1 sol_old{8};
    Move wrong_move = Move{67, 51};
    for (auto i = 0; i < 2; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,0,0,1,1,1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,0,1,1,0,1,-1,-1,-1,-1,-1,-1,-1,-1,
            // 48
            1,1,1,0,0,1,0,1,-1,-1,-1,-1,-1,-1,-1,-1,
            // 64
            0,3,0,3,3,3,3,1,-1,-1,-1,-1,-1,-1,-1,-1,
            3,3,3,3,3,3,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,0,0,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        // Move move = sol.get_best_move(board, false);
        Move move = sol_old.get_best_move(board, false);
        // assert (move != wrong_move);
        json xx = {{"move", move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
    }
}

void test14()
{
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,1,1,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,1,1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,0,0,3,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,3,3,3,3,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,0,3,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };
    int point = MySolution::calc_board(board, false);
    assert (point > solider_point);
}

void test15(MySolution &sol)
{
    Move wrong_move = Move{48, 32};
    for (auto i = 0; i < 2; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            3,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        Move move = sol.get_best_move(board, false);
        sol.print_status();
        assert (move != wrong_move);
        json xx = {{"move", move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
    }
}

void test16(MySolution &sol)
{
    Move wrong_move = Move{101, 85};
    for (auto i = 0; i < 2; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,1,0,1,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,1,1,1,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
            3,1,1,0,0,1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            3,0,0,3,3,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,3,0,0,3,3,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        Move move = sol.get_best_move(board, false);
        sol.print_status();
        json xx = {{"move", move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
        // assert (move != wrong_move);
    }
}

void test17(MySolution &sol)
{
    Board board = {
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        1,1,1,1,0,1,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,1,0,1,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        3,1,1,0,0,1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,0,0,3,3,0,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,3,3,3,3,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,3,0,0,3,3,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };
    Move move = sol.get_best_move(board, false);
    int_fast64_t hash_key = 0;
    sol.calc_hash_key(board, hash_key);
    int_fast64_t hash_key_tmp = hash_key;
    MoveOps ops;
    sol.do_move2(board, move, ops, hash_key, false);
    sol.undo_move2(board, ops, hash_key);
    assert (hash_key == hash_key_tmp);
    sol.print_status();
    json xx = {{"move", move}};
    std::cout << xx.dump() << std::endl;
}

void test18(MySolution &sol)
{
    Move wrong_move = Move{84, 68};
    for (auto i = 0; i < 2; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,0,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,1,1,1,0,1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,1,0,0,0,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
            3,3,3,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            3,0,0,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        MySolutionV1 old_sol{3};
        MySolution new_dol{3, 500, 3};
        Move move_old = old_sol.get_best_move(board, false);
        Move move = new_dol.get_best_move(board, false);
        MySolution::print_board(board);
        sol.print_status();
        json xx = {{"new_move", move, "move_old", move_old}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
        assert (move != wrong_move);
    }
}

void test19(MySolution &sol)
{
    Board board = {
        2,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,3,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };
    MySolution::print_board(board);
    Move move_1 = sol.get_best_move(board, false);
    MySolution::print_board(board);
    int_fast64_t hash_key = 0;
    MoveOps ops;
    Move move = {53, 37};
    sol.do_move2(board, move, ops, hash_key, false);
    MySolution::print_board(board);
    std::cout<<"asdf"<<std::endl;
    assert (board[0] == white_king);
}

void test20() {
    // 测试 hash key 算法的正确性
    MySolution sol_white{6, 3000, 6};
    MySolution sol_black{6, 3000, 6};
    Board board = {
        0,2,0,2,0,2,0,2,-1,-1,-1,-1,-1,-1,-1,-1,
        2,0,1,0,1,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,1,0,1,0,1,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,4,-1,-1,-1,-1,-1,-1,-1,-1,
        3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
        0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
    };
    int_fast64_t hash_key1 = 0;
    int_fast64_t hash_key2 = 0;
    int_fast64_t hash_key3 = 0;
    MySolution::calc_hash_key(board, hash_key1);
    Move white_move = sol_white.get_best_move(board, true);
    MySolution::calc_hash_key(board, hash_key2);
    Move black_move = sol_black.get_best_move(board, false);
    MySolution::calc_hash_key(board, hash_key3);
    assert (hash_key1 == hash_key2 && hash_key2 == hash_key3);
}

void test21()
{
    Move wrong_move = Move{83, 99};
    for (auto i = 0; i < 2; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,0,0,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,0,1,1,0,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,1,1,1,1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            3,3,0,0,0,0,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,3,3,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,3,0,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        MySolutionV1 old_sol{8};
        MySolution new_sol{16, 5000, 8};
        Move old_move = old_sol.get_best_move(board, false);
        Move new_move = new_sol.get_best_move(board, false);
        MySolution::print_board(board);
        new_sol.print_status();
        json xx = {{"new_move", new_move, "move_old", old_move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
        assert (new_move != wrong_move);
    }
    std::cout<<"test21"<<std::endl;
}

void test22()
{
    // [info][game_id: 74dd8c93-224c-41b0-ad89-dfb0ed852e8f, white: true, ai_name: 10] {"msg":"stat:\n{\"best_point\":0,\"board_point\":0,\"is_break\":true,\"is_white\":true,\"max_step_ms\":10,\"move\":[32,48],\"real_depth\":4,\"round\":4,\"time_in_ms\":11,\"try_depth\":6,\"version\":\"hehe_v1_6\"}"}
    Move wrong_move = Move{32, 48};
    for (auto i = 0; i < 10; ++i) {
        Board board = {
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            1,1,0,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            3,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
            0,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        MySolutionV1 old_sol{4};
        MySolution new_sol{20, 8, 2};
        Move old_move = old_sol.get_best_move(board, true);
        Move new_move = new_sol.get_best_move(board, true);
        MySolution::print_board(board);
        new_sol.print_status();
        json xx = {{"new_move", new_move, "move_old", old_move}};
        std::cout << i << std::endl;
        std::cout << xx.dump() << std::endl;
        assert (new_move != wrong_move);
    }
    std::cout<<"test22"<<std::endl;
}

void test23()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    std::mt19937 g(rd());
    auto rng = std::default_random_engine {};

    for (int i = 0; i < 99; ++i) {
        auto a = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        std::shuffle(std::begin(a), std::end(a), rng);
        for (auto v : a) {
            std::cout<<v<<" ";
        }
        std::cout<<std::endl;
    }
}

void run_test()
{
    MySolution sol{2, 30000, 2};
    // test1(sol);
    // test2(sol);
    // test3(sol);
    // test4(sol);
    // test5(sol);
    // test6(sol);
    // test7(sol);
    // test8(sol);
    // test9(sol);
    // test10(sol);
    // 移动没出过问题， 测试不维护了。

    // test12(sol);
    // test13();
    // test14();
    // test15(sol);
    // test16(sol);
    // test17(sol);
    // test18(sol);
    // test19(sol);
    // test20();
    // test21();
    // test22();
    test23();
}