#pragma once

#include "bitboard.hpp"

class Controller;
class ChessBoard
{
    friend class Controller;

public:
    ChessBoard();
    ChessBoard Copy() const;
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
