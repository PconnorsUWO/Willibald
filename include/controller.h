//
// Created by Alexander King Perocho on 2025-03-18.
//

#pragma once

#include "chessboard.h"
#include "move.h"
#include "fen.h"

class Controller
{
public:
    explicit Controller(Chessboard& chessboard);
    void MakeMove(const Move& move, int color) const;
    void ParseFen(const std::string& fen = Fen::START_POSITION) const;
    [[nodiscard]] bool IsLegalPosition(Bitboard occupancy) const;

private:
    Chessboard& chessboard;
};
