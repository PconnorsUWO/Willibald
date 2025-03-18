//
// Created by Alexander King Perocho on 2025-03-10.
//
#pragma once
#include "bitboard.h"

class Chessboard
{
    friend class Controller;

public:
    Chessboard();
    [[nodiscard]] Chessboard Copy() const;
    [[nodiscard]] Bitboard GetPieceOccupancy(int piece) const;
    [[nodiscard]] Bitboard GetColorOccupancy(int color) const;
    [[nodiscard]] int GetSideToMove() const;
    [[nodiscard]] int GetEnpassantSquare() const;
    [[nodiscard]] int GetCastlePrivilege() const;

private:
    Bitboard piece_occupancies[12];
    Bitboard color_occupancies[3];
    Bitboard zobrist_hash;
    int enpassant_square;
    int castle_privilege;
    int side_to_move;
};
