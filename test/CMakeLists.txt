Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/lib/catch2
)
FetchContent_MakeAvailable(Catch2)

# test target for unit testing
add_executable(units
main.test.cpp
attacks.test.cpp
bitboard.test.cpp
board.test.cpp
)
target_link_libraries(units PRIVATE Catch2::Catch2WithMain engine common)
target_include_directories(units PRIVATE ${CMAKE_SOURCE_DIR}/src/engine)

# separate test target for perft test
add_executable(perft
  main.test.cpp
  perft.test.cpp
)
target_link_libraries(perft PRIVATE Catch2::Catch2WithMain engine common)