cmake_minimum_required(VERSION 3.10)
project(dama)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# g++ -fdiagnostics-color=always -g main.cpp solution.cpp unit_test.cpp -o main -l curl -I ./ -I include -std=c++11
find_package(CURL REQUIRED)

add_executable(main_debug EXCLUDE_FROM_ALL main solution unit_test my_sol)
target_link_libraries(main_debug CURL::libcurl)
target_include_directories(main_debug PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_executable(main_release EXCLUDE_FROM_ALL main solution unit_test my_sol)
target_link_libraries(main_release CURL::libcurl)
target_include_directories(main_release PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)