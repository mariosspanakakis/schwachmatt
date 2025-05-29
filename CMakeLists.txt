cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
project(Schwachmatt)

# fetch googletest from remote repository
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/lib/googletest
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# setup application target
add_executable(Engine
    src/main.cpp
    src/attacks.cpp
    src/board.cpp
    src/bitboard.cpp
    src/engine.cpp
    src/move.cpp
    src/movegen.cpp
    src/uci.cpp
    src/utils.cpp
)
set_target_properties(Engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

# setup test target
add_executable(Test
    src/test.cpp
    src/attacks.cpp
    src/board.cpp
    src/bitboard.cpp
    src/engine.cpp
    src/move.cpp
    src/movegen.cpp
    src/uci.cpp
    src/utils.cpp
)

# setup perft test target
add_executable(Perft
    test/perft.test.cpp
    src/attacks.cpp
    src/bitboard.cpp
    src/board.cpp
    src/move.cpp
    src/movegen.cpp
    src/utils.cpp
)
target_include_directories(Perft PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Perft gtest gtest_main)

# setup target for unit tests
enable_testing()
add_executable(UnitTests
    test/attacks.test.cpp
    test/bitboard.test.cpp
    test/board.test.cpp
    src/utils.cpp
    src/move.cpp
    src/movegen.cpp
)
target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(UnitTests gtest gtest_main)
gtest_discover_tests(UnitTests)