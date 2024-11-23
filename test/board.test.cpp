#include <catch2/catch_test_macros.hpp>
#include "movegen.h"
#include "board.cpp"

TEST_CASE("Board: get piece information") {
    // get piece type
    REQUIRE(type_of(WHITE_PAWN) == PAWN);
    REQUIRE(type_of(WHITE_KNIGHT) == KNIGHT);
    REQUIRE(type_of(WHITE_BISHOP) == BISHOP);
    REQUIRE(type_of(WHITE_ROOK) == ROOK);
    REQUIRE(type_of(WHITE_QUEEN) == QUEEN);
    REQUIRE(type_of(WHITE_KING) == KING);
    REQUIRE(type_of(BLACK_PAWN) == PAWN);
    REQUIRE(type_of(BLACK_KNIGHT) == KNIGHT);
    REQUIRE(type_of(BLACK_BISHOP) == BISHOP);
    REQUIRE(type_of(BLACK_ROOK) == ROOK);
    REQUIRE(type_of(BLACK_QUEEN) == QUEEN);
    REQUIRE(type_of(BLACK_KING) == KING);
    // get piece color
    REQUIRE(color_of(WHITE_PAWN) == WHITE);
    REQUIRE(color_of(WHITE_KNIGHT) == WHITE);
    REQUIRE(color_of(WHITE_BISHOP) == WHITE);
    REQUIRE(color_of(WHITE_ROOK) == WHITE);
    REQUIRE(color_of(WHITE_QUEEN) == WHITE);
    REQUIRE(color_of(WHITE_KING) == WHITE);
    REQUIRE(color_of(BLACK_PAWN) == BLACK);
    REQUIRE(color_of(BLACK_KNIGHT) == BLACK);
    REQUIRE(color_of(BLACK_BISHOP) == BLACK);
    REQUIRE(color_of(BLACK_ROOK) == BLACK);
    REQUIRE(color_of(BLACK_QUEEN) == BLACK);
    REQUIRE(color_of(BLACK_KING) == BLACK);
    // make piece
    REQUIRE(make_piece(WHITE, PAWN) == WHITE_PAWN);
    REQUIRE(make_piece(WHITE, KNIGHT) == WHITE_KNIGHT);
    REQUIRE(make_piece(WHITE, BISHOP) == WHITE_BISHOP);
    REQUIRE(make_piece(WHITE, ROOK) == WHITE_ROOK);
    REQUIRE(make_piece(WHITE, QUEEN) == WHITE_QUEEN);
    REQUIRE(make_piece(WHITE, KING) == WHITE_KING);
    REQUIRE(make_piece(BLACK, PAWN) == BLACK_PAWN);
    REQUIRE(make_piece(BLACK, KNIGHT) == BLACK_KNIGHT);
    REQUIRE(make_piece(BLACK, BISHOP) == BLACK_BISHOP);
    REQUIRE(make_piece(BLACK, ROOK) == BLACK_ROOK);
    REQUIRE(make_piece(BLACK, QUEEN) == BLACK_QUEEN);
    REQUIRE(make_piece(BLACK, KING) == BLACK_KING);
}

TEST_CASE("Board: set and unset piece") {
    Board originalBoard = Board();
    Board board = Board();

    for (Color color : {WHITE, BLACK}) {
        for (PieceType pieceType = PAWN; pieceType < N_PIECE_TYPES; pieceType ++) {
            for (Square square = 16; square < 48; square++) {
                board.setPiece(square, make_piece(color, pieceType));
                board.unsetPiece(square);
            }
            REQUIRE(originalBoard.getPieceOccupancy(pieceType, color) == board.getPieceOccupancy(pieceType, color));
        }
        REQUIRE(originalBoard.getColorOccupancy(color) == board.getColorOccupancy(color));
    }
    REQUIRE(originalBoard.getTotalOccupancy() == board.getTotalOccupancy());
}

// TODO: add more depth to also test for more complex moves
TEST_CASE("Board: make and unmake moves") {
    Board originalBoard = Board();
    Board board = Board();

    MoveList movelist = MoveList(board);
    for (const Move& move : movelist) {
        board.makeMove(move);
        board.unmakeMove(move);
        for (Color color : {WHITE, BLACK}) {
            for (PieceType pieceType = PAWN; pieceType < N_PIECE_TYPES; pieceType ++) {
                REQUIRE(board.getPieceOccupancy(pieceType, color) == originalBoard.getPieceOccupancy(pieceType, color));
            }
            REQUIRE(board.getColorOccupancy(color) == originalBoard.getColorOccupancy(color));
        }
        REQUIRE(board.getTotalOccupancy() == originalBoard.getTotalOccupancy());

        for (Square sq = 0; sq < N_SQUARES; sq++) {
            REQUIRE(board.getPieceOnSquare(sq) == originalBoard.getPieceOnSquare(sq));
        }

        // TODO: add test cases for castling rights
    }
}