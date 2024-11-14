#include <gtest/gtest.h>
#include "board.cpp"
#include "movegen.cpp"


TEST(BoardTest, GetPieceInformation) {
    using namespace chess;
    // get piece type
    ASSERT_EQ(type_of(WHITE_PAWN), PAWN);
    ASSERT_EQ(type_of(WHITE_KNIGHT), KNIGHT);
    ASSERT_EQ(type_of(WHITE_BISHOP), BISHOP);
    ASSERT_EQ(type_of(WHITE_ROOK), ROOK);
    ASSERT_EQ(type_of(WHITE_QUEEN), QUEEN);
    ASSERT_EQ(type_of(WHITE_KING), KING);
    ASSERT_EQ(type_of(BLACK_PAWN), PAWN);
    ASSERT_EQ(type_of(BLACK_KNIGHT), KNIGHT);
    ASSERT_EQ(type_of(BLACK_BISHOP), BISHOP);
    ASSERT_EQ(type_of(BLACK_ROOK), ROOK);
    ASSERT_EQ(type_of(BLACK_QUEEN), QUEEN);
    ASSERT_EQ(type_of(BLACK_KING), KING);
    // get piece color
    ASSERT_EQ(color_of(WHITE_PAWN), WHITE);
    ASSERT_EQ(color_of(WHITE_KNIGHT), WHITE);
    ASSERT_EQ(color_of(WHITE_BISHOP), WHITE);
    ASSERT_EQ(color_of(WHITE_ROOK), WHITE);
    ASSERT_EQ(color_of(WHITE_QUEEN), WHITE);
    ASSERT_EQ(color_of(WHITE_KING), WHITE);
    ASSERT_EQ(color_of(BLACK_PAWN), BLACK);
    ASSERT_EQ(color_of(BLACK_KNIGHT), BLACK);
    ASSERT_EQ(color_of(BLACK_BISHOP), BLACK);
    ASSERT_EQ(color_of(BLACK_ROOK), BLACK);
    ASSERT_EQ(color_of(BLACK_QUEEN), BLACK);
    ASSERT_EQ(color_of(BLACK_KING), BLACK);
    // make piece
    ASSERT_EQ(make_piece(WHITE, PAWN), WHITE_PAWN);
    ASSERT_EQ(make_piece(WHITE, KNIGHT), WHITE_KNIGHT);
    ASSERT_EQ(make_piece(WHITE, BISHOP), WHITE_BISHOP);
    ASSERT_EQ(make_piece(WHITE, ROOK), WHITE_ROOK);
    ASSERT_EQ(make_piece(WHITE, QUEEN), WHITE_QUEEN);
    ASSERT_EQ(make_piece(WHITE, KING), WHITE_KING);
    ASSERT_EQ(make_piece(BLACK, PAWN), BLACK_PAWN);
    ASSERT_EQ(make_piece(BLACK, KNIGHT), BLACK_KNIGHT);
    ASSERT_EQ(make_piece(BLACK, BISHOP), BLACK_BISHOP);
    ASSERT_EQ(make_piece(BLACK, ROOK), BLACK_ROOK);
    ASSERT_EQ(make_piece(BLACK, QUEEN), BLACK_QUEEN);
    ASSERT_EQ(make_piece(BLACK, KING), BLACK_KING);
}

TEST(BoardTest, SetAndUnsetPiece) {
    chess::Board originalBoard = chess::Board();
    chess::Board board = chess::Board();

    for (chess::Color color : {chess::WHITE, chess::BLACK}) {
        for (chess::PieceType pieceType = chess::PAWN; pieceType < chess::N_PIECE_TYPES; pieceType ++) {
            for (chess::Square square = 16; square < 48; square++) {
                board.setPiece(square, chess::make_piece(color, pieceType));
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