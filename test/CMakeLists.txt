include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/lib/googletest
)
FetchContent_MakeAvailable(googletest)

# test target for unit testing
add_executable(unittests
  main.test.cpp
  attacks.test.cpp
  bitboard.test.cpp
  board.test.cpp
)
target_link_libraries(unittests PRIVATE GTest::gtest_main engine common)
target_include_directories(unittests PRIVATE ${CMAKE_SOURCE_DIR}/src/engine)

# separate test target for perft test
add_executable(perft
  main.test.cpp
  perft.test.cpp
)
target_link_libraries(perft PRIVATE GTest::gtest_main engine common)

# Enable GoogleTest's CTest integration (optional)
include(GoogleTest)
gtest_discover_tests(unittests)
gtest_discover_tests(perft)
