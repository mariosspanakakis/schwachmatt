#include <gtest/gtest.h>
#include "board.h"
#include "movegen.h"

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

// TODO: add more depth to also test for more complex moves
TEST(BoardTest, MakeAndUnmakeMove) {
    chess::Board originalBoard = chess::Board();
    chess::Board board = chess::Board();

    chess::MoveList movelist = chess::MoveList(board);
    for (const chess::Move& move : movelist) {
        board.makeMove(move);
        board.unmakeMove(move);
        for (chess::Color color : {chess::WHITE, chess::BLACK}) {
            for (chess::PieceType pieceType = chess::PAWN; pieceType < chess::N_PIECE_TYPES; pieceType ++) {
                ASSERT_EQ(board.getPieceOccupancy(pieceType, color), originalBoard.getPieceOccupancy(pieceType, color));
            }
            ASSERT_EQ(board.getColorOccupancy(color), originalBoard.getColorOccupancy(color));
        }
        ASSERT_EQ(board.getTotalOccupancy(), originalBoard.getTotalOccupancy());

        for (chess::Square sq = 0; sq < chess::N_SQUARES; sq++) {
            ASSERT_EQ(board.getPieceOnSquare(sq), originalBoard.getPieceOnSquare(sq));
        }

        // TODO: add test cases for castling rights
    }
}