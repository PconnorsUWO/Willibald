//
// Created by Alexander King Perocho on 2025-03-18.
//
#pragma once

#include <vector>
#include "chessboard.h"
#include "move.h"

namespace Log
{
    void PrintBitboard(Bitboard bitboard);
    void PrintPieceOccupancy(const Chessboard& chessboard, int piece);
    void PrintChessboard(const Chessboard& chessboard);
    void PrintAttackedSquares(const Chessboard& chessboard, int color);
    void PrintMoves(const Chessboard& chessboard, int color);
    void PrintPieceMoves(const Chessboard& chessboard, int piece, int color);
    void PrintMoveEncoding(const Move& move);
    void PrintMovesEncoding(const std::vector<Move>& moves);
}
