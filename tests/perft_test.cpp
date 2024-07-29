#include <iomanip>
#include <iostream>
#include <vector>
#include <chrono>
#include <gtest/gtest.h>
#include "board.h"
#include "movegen.h"

struct PerftTestCase {
    std::string fen;
    std::vector<uint64_t> nodes;
};

std::vector<PerftTestCase> PERFT_TEST_CASES = {
    // position 1: standard initial position
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 
        {20, 400, 8902, 197281, 4865609, /*119060324*/}
    },
    // position 2: Kiwipete
    {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
        {48, 2039, 97862, 4085603, 193690690, /*8031647685*/}
    },
    // position 3
    {
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
        {14, 191, 2812, 43238, 674624, /*11030083*/}
    }
};

uint64_t perft(chess::Board board, int depth) {

    uint64_t nodes = 0;
    
    // generate all pseudo-legal moves
    chess::MoveList movelist = chess::MoveList(board);

    // end search if base depth has been reached
    if (depth == 0) {
        return 1ULL;
    }

    for (const chess::Move& move : movelist) {
        // make move
        board.makeMove(move);
        
        // check for legality of the move
        if (!board.isInCheck(!board.getSideToMove())) {
            nodes += perft(board, depth - 1);
        }

        // unmake move
        board.unmakeMove(move);
    }
    return nodes;
}

TEST(PerftTest, Perft) {
    chess::attacks::initializeAttackTables();

    bool success = true;

    std::cout << std::fixed << std::setprecision(2);

    for (PerftTestCase test_case : PERFT_TEST_CASES) {

        std::vector<uint64_t> expected_values = test_case.nodes;
        std::string fen = test_case.fen;

        std::cout << std::endl;
        std::cout << "FEN: " << fen << std::endl;
        std::cout << std::endl;
        std::cout << "Depth"
                << std::setw(16) << "Expected"
                << std::setw(16) << "Result"
                << std::setw(10) << "Time [s]"
                << std::setw(16) << "Speed [nps]"
                << std::endl;

        for (size_t depth = 1; depth < expected_values.size() + 1; ++depth) {
            chess::Board board = chess::Board(fen);

            auto start = std::chrono::high_resolution_clock::now();
            uint64_t result = perft(board, depth);
            auto end = std::chrono::high_resolution_clock::now();

            uint64_t expected = expected_values[depth - 1];
            std::chrono::duration<double, std::milli> elapsed = end - start;

            success = result == expected;
            std::string msg = success ? "" : "failed";

            std::cout << std::setw(5) << depth 
                    << std::setw(16) << expected 
                    << std::setw(16) << result 
                    << std::setw(10) << elapsed.count() / 1000.0
                    << std::setw(16) << result / elapsed.count() * 1000
                    << std::setw(8) << msg << std::endl;
        }

        std::cout << std::endl;
    }

    EXPECT_TRUE(success);
}