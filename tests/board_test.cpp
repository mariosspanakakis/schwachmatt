#include <gtest/gtest.h>
#include "board.h"
#include "movegen.h"


TEST(BoardTest, GetPieceInformation) {
    using namespace chess;
    // get piece type
    ASSERT_EQ(typeOf(WHITE_PAWN), PAWN);
    ASSERT_EQ(typeOf(WHITE_KNIGHT), KNIGHT);
    ASSERT_EQ(typeOf(WHITE_BISHOP), BISHOP);
    ASSERT_EQ(typeOf(WHITE_ROOK), ROOK);
    ASSERT_EQ(typeOf(WHITE_QUEEN), QUEEN);
    ASSERT_EQ(typeOf(WHITE_KING), KING);
    ASSERT_EQ(typeOf(BLACK_PAWN), PAWN);
    ASSERT_EQ(typeOf(BLACK_KNIGHT), KNIGHT);
    ASSERT_EQ(typeOf(BLACK_BISHOP), BISHOP);
    ASSERT_EQ(typeOf(BLACK_ROOK), ROOK);
    ASSERT_EQ(typeOf(BLACK_QUEEN), QUEEN);
    ASSERT_EQ(typeOf(BLACK_KING), KING);
    // get piece color
    ASSERT_EQ(colorOf(WHITE_PAWN), WHITE);
    ASSERT_EQ(colorOf(WHITE_KNIGHT), WHITE);
    ASSERT_EQ(colorOf(WHITE_BISHOP), WHITE);
    ASSERT_EQ(colorOf(WHITE_ROOK), WHITE);
    ASSERT_EQ(colorOf(WHITE_QUEEN), WHITE);
    ASSERT_EQ(colorOf(WHITE_KING), WHITE);
    ASSERT_EQ(colorOf(BLACK_PAWN), BLACK);
    ASSERT_EQ(colorOf(BLACK_KNIGHT), BLACK);
    ASSERT_EQ(colorOf(BLACK_BISHOP), BLACK);
    ASSERT_EQ(colorOf(BLACK_ROOK), BLACK);
    ASSERT_EQ(colorOf(BLACK_QUEEN), BLACK);
    ASSERT_EQ(colorOf(BLACK_KING), BLACK);
    // make piece
    ASSERT_EQ(makePiece(WHITE, PAWN), WHITE_PAWN);
    ASSERT_EQ(makePiece(WHITE, KNIGHT), WHITE_KNIGHT);
    ASSERT_EQ(makePiece(WHITE, BISHOP), WHITE_BISHOP);
    ASSERT_EQ(makePiece(WHITE, ROOK), WHITE_ROOK);
    ASSERT_EQ(makePiece(WHITE, QUEEN), WHITE_QUEEN);
    ASSERT_EQ(makePiece(WHITE, KING), WHITE_KING);
    ASSERT_EQ(makePiece(BLACK, PAWN), BLACK_PAWN);
    ASSERT_EQ(makePiece(BLACK, KNIGHT), BLACK_KNIGHT);
    ASSERT_EQ(makePiece(BLACK, BISHOP), BLACK_BISHOP);
    ASSERT_EQ(makePiece(BLACK, ROOK), BLACK_ROOK);
    ASSERT_EQ(makePiece(BLACK, QUEEN), BLACK_QUEEN);
    ASSERT_EQ(makePiece(BLACK, KING), BLACK_KING);
}

TEST(BoardTest, SetAndUnsetPiece) {
    chess::Board originalBoard = chess::Board();
    chess::Board board = chess::Board();

    for (chess::Color color : {chess::WHITE, chess::BLACK}) {
        for (chess::PieceType pieceType = chess::PAWN; pieceType < chess::N_PIECE_TYPES; pieceType ++) {
            for (chess::Square square = 16; square < 48; square++) {
                board.setPiece(square, chess::makePiece(color, pieceType));
                board.unsetPiece(square);
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