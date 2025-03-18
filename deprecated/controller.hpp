#pragma once

#include "chess_board.hpp"
#include "move.hpp"
#include "FEN.hpp"

class Controller
{
public:
    Controller(ChessBoard &chess_board);
    void MakeMove(Move move, int color);
    void ParseFen(std::string fen = Fen::START_POSITION);
    bool IsLegalPosition(Bitboard occupancy) const;

private:
    ChessBoard &chess_board;
};