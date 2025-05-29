#ifndef MOVE_HPP
#define MOVE_HPP

#include <bitset>
#include <cstdint>
#include <iostream>
#include <map>
#include "bitboard.hpp"

/*
The 16 bits of a move are assigned as follows:
    0000 | 000000 | 000000
    FLAG     TO      FROM

Note:
This move representation does not specify the moving or captured piece. It is
therefore necessary to look at the board that the move has been made on in
order to obtain this information.
*/

class Move {
    private:
    uint16_t move_;

    public:
    Move();
    Move(Square from, Square to, MoveFlag flag);
    ~Move() = default;
    inline Square getTo() const { return (move_ >> TO_SHIFT) & 0x3f; };
    inline void setTo(Square to) { move_ |= (to << TO_SHIFT); };
    inline Square getFrom() const { return (move_ >> FROM_SHIFT) & 0x3f; };
    inline void setFrom(Square from) { move_ |= (from << FROM_SHIFT); };
    inline MoveFlag getFlag() const { return (move_ >> FLAG_SHIFT) & 0b1111; };
    inline void setFlag(MoveFlag flag) { move_ |= (flag << FLAG_SHIFT); };

    inline bool isCapture() { return (move_ >> FLAG_SHIFT) & 0b0100; };
    inline bool isDoublePawnPush() { return (move_ >> FLAG_SHIFT) == 0b0001; };
    inline bool isEnPassantCapture() { return (move_ >> FLAG_SHIFT) == 0b0101; };
    inline bool isPromotion() { return (move_ >> FLAG_SHIFT) & 0b1000; };
    inline PieceType getPromotionPieceType() { return PieceType(((move_ >> FLAG_SHIFT) & 0b0011) + 2);};
    inline bool isCastling() { return (getFlag() == KINGSIDE_CASTLE || getFlag() == QUEENSIDE_CASTLE);};
    
    std::string toString() const;
    void printDetails() const;
};

#endif
