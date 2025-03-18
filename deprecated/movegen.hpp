#pragma once

#include <vector>
#include "chess_board.hpp"
#include "move.hpp"

class MoveGen
{
public:
    static bool IsAttacked(const ChessBoard chess_board, const int square, const int color);
    static std::vector<Move> GetPawnMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetKingMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetKnightMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetBishopMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetRookMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetQueenMoves(const ChessBoard chess_board, const int color);
    static std::vector<Move> GetMoves(const ChessBoard chess_board, const int color);
};

/*
          binary move encoding                           hexidecimal constants
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/