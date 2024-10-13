cmake_minimum_required(VERSION 3.10)
project(dama)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# g++ -fdiagnostics-color=always -g main.cpp solution.cpp unit_test.cpp -o main -l curl -I ./ -I include -std=c++11
add_executable(main EXCLUDE_FROM_ALL main solution unit_test my_sol)
find_package(CURL REQUIRED)
target_link_libraries(main CURL::libcurl)
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)