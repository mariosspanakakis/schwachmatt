add_library(engine STATIC
    attacks.cpp
    bitboard.cpp
    board.cpp
    move.cpp
    movegen.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_include_directories(engine PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_link_libraries(engine PRIVATE common)