#include <iostream>
#include <gtest/gtest.h>
#include "board.h"
#include "bitboard.h"
#include "attacks.h"


/* --- BITBOARD UNIT TESTS --- */
/* Set a bit on an empty bitboard and test for the result. */
TEST(BitboardTest, SetBit) {
    chess::Bitboard bitboard = 0ULL;
    chess::bb::setBit(bitboard, chess::E4);
    chess::Bitboard expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

/* Clear a bit on a bitboard and test for the result. */
TEST(BitboardTest, ClearBit) {
    chess::Bitboard bitboard = 268435456;
    chess::Bitboard expected = 0ULL;
    chess::bb::clearBit(bitboard, chess::E4);
    EXPECT_EQ(bitboard, expected);
}

/* --- ATTACK TABLE GENERATION UNIT TESTS --- */
/* Generate sliding piece attack tables and check for errors. */
TEST(AttackTest, MagicNumberGeneration) {
    chess::attacks::initializeAttackTables();

    for (int is_bishop = 0; is_bishop < 2; is_bishop++) {
        for (int square = 0; square < chess::N_SQUARES; square++) {
            // get attack mask and number of relevant bits
            chess::Bitboard attack_mask = is_bishop ? chess::attacks::BISHOP_MASK[square] : chess::attacks::ROOK_MASK[square];
            int bits = is_bishop ? chess::attacks::BISHOP_BITS[square] : chess::attacks::ROOK_BITS[square];

            // manually generate the attack mask for each blocker configuration
            for (int i = 0; i < (1 << bits); i++) {
                chess::Bitboard blockers = chess::attacks::getBlockerConfiguration(i, attack_mask);
                chess::Bitboard manual_attacks = is_bishop ? chess::attacks::calculateBishopAttacks(square, blockers) : chess::attacks::calculateRookAttacks(square, blockers);
                chess::Bitboard lookup_attacks = is_bishop ? chess::attacks::lookupBishopAttacks(square, blockers) : chess::attacks::lookupRookAttacks(square, blockers);

                ASSERT_EQ(manual_attacks, lookup_attacks);
            }
        }
    }
}

/* --- MOVE GENERATION UNIT TESTS --- */

/* --- BOARD UNIT TESTS --- */
TEST(BoardTest, SetAndUnsetPiece) {
    chess::Board originalBoard = chess::Board();
    chess::Board board = chess::Board();

    for (chess::Color color : {chess::WHITE, chess::BLACK}) {
        for (chess::PieceType pieceType = chess::PAWN; pieceType < chess::N_PIECE_TYPES; pieceType ++) {
            for (chess::Square square = 16; square < 48; square++) {
                board.setPiece(square, pieceType, color);
                board.unsetPiece(square, pieceType, color);
            }
            ASSERT_EQ(originalBoard.getPieceOccupancy(pieceType, color), board.getPieceOccupancy(pieceType, color));
        }
        ASSERT_EQ(originalBoard.getColorOccupancy(color), board.getColorOccupancy(color));
    }
    ASSERT_EQ(originalBoard.getTotalOccupancy(), board.getTotalOccupancy());
}

/* --- MAIN TEST FUNCTION --- */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}