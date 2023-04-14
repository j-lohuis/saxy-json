
cmake_minimum_required(VERSION 3.23)
project(saxy-json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra -Wconversion -Wpedantic)# -fsanitize=undefined,address)
add_link_options(-Wall -Wextra -Wconversion -Wpedantic)# -fsanitize=undefined,address)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION $<$<CONFIG:Release>:1:0>)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Catch2 REQUIRED)
add_executable(tests
    test/test.cc
    test/saxy-json.cc
)
target_link_libraries(tests PRIVATE Catch2::Catch2)

add_executable(parser test/parser.cc)

