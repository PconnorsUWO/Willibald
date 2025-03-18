#pragma once

#include "chess_board.hpp"
#include "move.hpp"
#include <vector>

namespace Log
{
    void PrintBitboard(Bitboard bitboard);
    void PrintPieceOccupancy(ChessBoard chess_board, int piece);
    void PrintChessBoard(ChessBoard chess_board);
    void PrintAttackedSquares(ChessBoard chess_board, int color);
    void PrintMoves(ChessBoard chess_board, int color);
    void PrintMoveEncoding(Move move);
    void PrintMovesEncoding(std::vector<Move> moves);
}