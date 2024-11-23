#include <catch2/catch_test_macros.hpp>
#include "bitboard.cpp"

TEST_CASE("Bitboard: setting bits") {
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, E4);
    Bitboard expected = 268435456;
    REQUIRE(bitboard == expected);
}

TEST_CASE("Bitboard: clearing bits") {
    Bitboard bitboard = 268435456;
    Bitboard expected = 0ULL;
    bb::clearBit(bitboard, E4);
    REQUIRE(bitboard == expected);
}