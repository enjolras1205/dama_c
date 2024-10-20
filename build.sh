# g++ -I ./ -o main main.cpp solution.cpp -lcurl -I include -std=c++11
g++ -Ofast -I ./ -I ./history -o main main.cpp solution.cpp unit_test.cpp my_sol.cpp local_battle.cpp history/my_sol_v1.cpp -lcurl -I include -std=c++11
