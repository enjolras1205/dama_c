cmake_minimum_required(VERSION 3.10)
project(dama)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# g++ -fdiagnostics-color=always -g main.cpp solution.cpp unit_test.cpp -o main -l curl -I ./ -I include -std=c++11
# find_package(CURL REQUIRED)

add_executable(main_debug EXCLUDE_FROM_ALL solution unit_test my_sol local_battle history/my_sol_v1 main.cpp)
target_link_libraries(main_debug curl)
target_include_directories(main_debug PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/history
)

add_executable(main_release EXCLUDE_FROM_ALL solution unit_test my_sol local_battle history/my_sol_v1 main.cpp)
target_link_libraries(main_release curl)
target_include_directories(main_release PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/history
)