//
// Created by Alexander King Perocho on 2025-03-10.
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "bitboard.h"

class Chessboard
{
    friend class Controller;

public:
    Chessboard();
    Chessboard Copy();
    Bitboard GetPieceOccupancy(const int piece) const;
    Bitboard GetColorOccupancy(const int color) const;
    int GetSideToMove() const;
    int GetEnpassantSquare() const;
    int GetCastlePrivelage() const;

private:
    Bitboard piece_occupancies[12];
    Bitboard color_occupancies[3];
    Bitboard zobrist_hash;
    int enpassant_square;
    int castle_privelage;
    int side_to_move;
};

#endif //CHESSBOARD_H
