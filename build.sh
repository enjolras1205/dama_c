# g++ -I ./ -o main main.cpp solution.cpp -lcurl -I include -std=c++11
g++ -fdiagnostics-color=always -g main.cpp solution.cpp unit_test.cpp -o main -l curl -I ./ -I include -std=c++11
