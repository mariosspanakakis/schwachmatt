cmake_minimum_required(VERSION 3.31)
project(schwachmatt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# create a library from the core source files
add_subdirectory(src/engine)
add_subdirectory(src/uci)

# generate the main executable
add_executable(main src/main.cpp)

# link the main executable with the required libraries
target_link_libraries(main PRIVATE engine uci)

# download and set up GoogleTest in lib/
include(FetchContent)
FetchContent_Declare(
    googletest
    URL             https://github.com/google/googletest/archive/b514bdc.zip
    SOURCE_DIR      ${CMAKE_SOURCE_DIR}/lib/googletest
)
# prevent overriding the parent project's compiler/linker settings (required)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# enable testing
enable_testing()

# create the unit test executable
add_executable(unit_tests
    test/attacks_test.cpp
    test/bitboard_test.cpp
    test/board_test.cpp
    test/main_test.cpp
)
# link unit test executable with the library and GoogleTest
target_link_libraries(unit_tests PRIVATE engine GTest::gtest_main)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
# discover and register unit tests
gtest_discover_tests(unit_tests)

# create the perft test executable
add_executable(perft test/perft_test.cpp)
# link perft test executable with the library and GoogleTest
target_link_libraries(perft PRIVATE engine GTest::gtest_main)
target_include_directories(perft PRIVATE ${CMAKE_SOURCE_DIR}/src)
# discover and register test cases for perft
gtest_discover_tests(perft)
