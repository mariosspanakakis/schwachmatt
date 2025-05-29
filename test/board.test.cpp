#include <gtest/gtest.h>
#include "movegen.hpp"
#include "board.cpp"

class BoardTest : public ::testing::Test {
protected:
    static constexpr PieceType pieceTypes[N_PIECE_TYPES] = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
    static constexpr Color colors[2] = {WHITE, BLACK};

    Board originalBoard;
    Board board;
};

TEST_F(BoardTest, GetPieceInformation) {
    for (Color color : colors) {
        for (PieceType pieceType : pieceTypes) {
            Piece piece = make_piece(color, pieceType);

            // check if make_piece correctly constructs the expected piece
            EXPECT_EQ(piece, (color == WHITE ? WHITE_PAWN : BLACK_PAWN) + pieceType - PAWN) << "make_piece failed for color: " << color << ", pieceType: " << pieceType;
            
            // check piece type extraction
            EXPECT_EQ(type_of(piece), pieceType) << "Failed for pieceType: " << pieceType;

            // check color extraction
            EXPECT_EQ(color_of(piece), color) << "Failed for color: " << color << ", pieceType: " << pieceType;
        }
    }
}

TEST_F(BoardTest, SetAndUnsetPiece) {
    for (Color color : colors) {
        for (PieceType pieceType : pieceTypes) {
            for (Square square = 16; square < 48; square++) {
                board.setPiece(square, make_piece(color, pieceType));
                board.unsetPiece(square);
            }
            EXPECT_EQ(originalBoard.getPieceOccupancy(pieceType, color), board.getPieceOccupancy(pieceType, color));
        }
        EXPECT_EQ(originalBoard.getColorOccupancy(color), board.getColorOccupancy(color));
    }
    EXPECT_EQ(originalBoard.getTotalOccupancy(), board.getTotalOccupancy());
}

TEST_F(BoardTest, MakeAndUnmakeMoves) {
    MoveList movelist(board);
    for (const Move& move : movelist) {
        board.makeMove(move);
        board.unmakeMove(move);

        for (Color color : colors) {
            for (PieceType pieceType : pieceTypes) {
                EXPECT_EQ(board.getPieceOccupancy(pieceType, color), originalBoard.getPieceOccupancy(pieceType, color));
            }
            EXPECT_EQ(board.getColorOccupancy(color), originalBoard.getColorOccupancy(color));
        }
        EXPECT_EQ(board.getTotalOccupancy(), originalBoard.getTotalOccupancy());

        for (Square sq = 0; sq < N_SQUARES; sq++) {
            EXPECT_EQ(board.getPieceOnSquare(sq), originalBoard.getPieceOnSquare(sq));
        }

        // TODO: add test cases for castling rights
    }
}
