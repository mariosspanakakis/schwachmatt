cmake_minimum_required(VERSION 3.31)
project(schwachmatt)

# use at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# generate main executable
add_executable(schwachmatt
    src/main.cpp
    src/main.h
    src/attacks.cpp
    src/attacks.h
    src/bitboard.cpp
    src/bitboard.h
    src/board.cpp
    src/board.h
    src/move.cpp
    src/move.h
    src/movegen.cpp
    src/movegen.h
    src/utils.cpp
    src/utils.h
    src/exceptions.h
    src/types.h
)

# download googletest into lib/
include(FetchContent)
FetchContent_Declare(
    googletest
    URL             https://github.com/google/googletest/archive/b514bdc.zip
    SOURCE_DIR      ${CMAKE_SOURCE_DIR}/lib/googletest
)
# prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# generate test executable
enable_testing()
add_executable(schwachmatt_test
    test/attacks_test.cpp
    test/bitboard_test.cpp
    test/board_test.cpp
    test/main_test.cpp
    test/perft_test.cpp
)

# include header files into the test executable
target_include_directories(schwachmatt_test PRIVATE ${CMAKE_SOURCE_DIR}/src)

# link the test executable with googletest library
target_link_libraries(schwachmatt_test GTest::gtest_main)

# activate automatic test discovery
gtest_discover_tests(schwachmatt_test)