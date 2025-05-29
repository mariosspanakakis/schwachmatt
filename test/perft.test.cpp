#include <gtest/gtest.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>
#include "board.hpp"
#include "movegen.hpp"

#define MAX_DEPTH 5
#define ENABLE_DETAILED_LOGGING 0

struct PerftTestCase {
    std::string fen;
    std::vector<uint64_t> nodes;
};

/* Standard perft test cases (https://www.chessprogramming.org/Perft_Results) */
std::vector<PerftTestCase> PERFT_TEST_CASES = {
    // position 1
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 
        {20, 400, 8902, 197281, 4865609, 119060324, 3195901860, 84998978956, 2439530234167}
    },
    // position 2
    {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
        {48, 2039, 97862, 4085603, 193690690, 8031647685}
    },
    // position 3
    {
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
        {14, 191, 2812, 43238, 674624, 11030083, 178633661, 3009794393}
    },
    // position 4
    {
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        //"r3k2r/Pppp1ppp/1b3nbN/nPP5/BB2P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",     // c4c5, fails after b2a1 rook promotion capture (probably due to something related to the castling rights!)
        {6, 264, 9467, 422333, 15833292, 706045033}                             // NOTE: currently fails at depth 6
    },
    // position 5
    {
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        {44, 1486, 62379, 2103487, 89941194}
    },
    // position 6
    {
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        {46, 2079, 89890, 3894594, 164075551, 6923051137, 287188994746, 11923589843526, 490154852788714}
    }
};

uint64_t perft(Board& board, int depth, bool enable_detailed_logging/*, std::string movestring = ""*/) {
    uint64_t nodes = 0;

    //std::cout << movestring << std::endl;
    
    // end search if base depth has been reached
    if (depth == 0) {
        return 1ULL;
    }

    // generate all pseudo-legal moves
    MoveList movelist = MoveList(board);

    for (const Move& move : movelist) {
        board.makeMove(move);
        
        // check for legality of the move
        if (!board.isInCheck(!board.getSideToMove())) {
            uint64_t childNodes = perft(board, depth - 1, false/*, movestring + "-" + move.toString()*/);
            nodes += childNodes;

            if (enable_detailed_logging && depth != 1) {
                std::cout << move.toString() << ": " << childNodes << std::endl;
            }
        }

        board.unmakeMove(move);
    }

    return nodes;
}

class PerftTest : public ::testing::Test {
protected:
    void SetUp() override {
        attacks::precalculate(true);
    }
};

TEST_F(PerftTest, MoveGeneration) {
    std::cout << std::fixed << std::setprecision(2);

    for (const PerftTestCase& test_case : PERFT_TEST_CASES) {
        // extract and print test case parameters
        std::vector<uint64_t> expected_values = test_case.nodes;
        std::string fen = test_case.fen;
        std::cout << std::endl;
        std::cout << "FEN: " << fen << std::endl;
        std::cout << std::endl;
        std::cout << std::setw(6)  << "Depth"
                << std::setw(16) << "Expected"
                << std::setw(16) << "Result"
                << std::setw(10) << "Time [s]"
                << std::setw(16) << "Speed [nps]"
                << std::endl;

        bool success = true;
        for (size_t depth = 1; depth <= MAX_DEPTH && depth <= expected_values.size(); ++depth) {
            Board board = Board(fen);

            auto start = std::chrono::high_resolution_clock::now();
            uint64_t result = perft(board, depth, ENABLE_DETAILED_LOGGING);
            auto end = std::chrono::high_resolution_clock::now();

            uint64_t expected = expected_values[depth - 1];
            std::chrono::duration<double, std::milli> elapsed = end - start;

            success = result == expected;
            std::string msg = success ? "" : "failed";

            std::cout << std::setw(6) << depth 
                    << std::setw(16) << expected 
                    << std::setw(16) << result 
                    << std::setw(10) << elapsed.count() / 1000.0
                    << std::setw(16) << result / elapsed.count() * 1000
                    << std::setw(8) << msg << std::endl;

            EXPECT_EQ(result, expected) << "Perft failed at depth " << depth << " for FEN: " << fen;
        }

        std::cout << std::endl;
    }
}
