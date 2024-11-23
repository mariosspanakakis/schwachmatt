add_subdirectory(engine)
add_subdirectory(uci)
add_subdirectory(common)

add_executable(schwachmatt main.cpp)

target_link_libraries(schwachmatt PRIVATE engine uci common)