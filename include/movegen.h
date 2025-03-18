//
// Created by Alexander King Perocho on 2025-03-18.
//
#pragma once
#include <vector>
#include "chessboard.h"
#include "move.h"

class MoveGen
{
public:
    static bool IsAttacked(const Chessboard& chessboard, int square, int color);
    static std::vector<Move> GetPawnMoves(const Chessboard& chessboard, int color);
    static std::vector<Move> GetKingMoves(Chessboard chessboard, int color);
    static std::vector<Move> GetKnightMoves(const Chessboard& chessboard, int color);
    static std::vector<Move> GetBishopMoves(const Chessboard& chessboard, int color);
    static std::vector<Move> GetRookMoves(const Chessboard& chessboard, int color);
    static std::vector<Move> GetQueenMoves(const Chessboard& chessboard, int color);
    static std::vector<Move> GetMoves(const Chessboard& chessboard, int color);

private:
    MoveGen();
};

/*
    binary move encoding             hexadecimal ants
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/
