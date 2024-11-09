# define the UCI library
add_library(uci STATIC uci.cpp)

# specify include directories for the UCI library (if uci.h is needed elsewhere)
target_include_directories(uci PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
