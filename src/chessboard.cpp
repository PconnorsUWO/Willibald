//
// Created by Alexander King Perocho on 2025-03-10.
//

#include <algorithm>
#include "../include/chessboard.h"
#include "../include/chess.h"

Chessboard::Chessboard()
{
    enpassant_square = Chess::NO_SQUARE;
    castle_privilege = Chess::NO_CASTLE;
    side_to_move = 0;
}

Chessboard Chessboard::Copy() const { return *this; }
Bitboard Chessboard::GetPieceOccupancy(const int piece) const { return piece_occupancies[piece]; }
Bitboard Chessboard::GetColorOccupancy(const int color) const { return color_occupancies[color]; }
int Chessboard::GetSideToMove() const { return side_to_move; }
int Chessboard::GetEnpassantSquare() const { return enpassant_square; }
int Chessboard::GetCastlePrivilege() const { return castle_privilege; }
