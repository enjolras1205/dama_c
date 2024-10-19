# g++ -I ./ -o main main.cpp solution.cpp -lcurl -I include -std=c++11
g++ -O3 -I ./ -I ./history -o main main.cpp solution.cpp unit_test.cpp my_sol.cpp local_battle.cpp history/my_sol_v1.cpp -lcurl -I include -std=c++11
