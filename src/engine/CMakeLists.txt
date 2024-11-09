# define the engine library
add_library(engine STATIC
    attacks.cpp
    bitboard.cpp
    board.cpp
    move.cpp
    movegen.cpp
    utils.cpp
)

# specify include directories for the engine library (for headers like exceptions.h and types.h)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
