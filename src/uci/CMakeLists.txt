add_library(uci
    uci.cpp
    uci.h
)

target_include_directories(uci PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(uci PRIVATE common)